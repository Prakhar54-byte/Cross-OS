#include "backend.h"
#include <QDesktopServices>

Backend::Backend(QObject *parent)
    : QObject(parent),
      m_clipboardSync(false),
      m_lanSync(false)
{
    loadSettings();

    // Setup TCP server for receiving incoming notes
    m_tcpServer = new QTcpServer(this);
    connect(m_tcpServer, &QTcpServer::newConnection, this, &Backend::handleNewConnection);
    // Start listening on any free port
    if (m_tcpServer->listen(QHostAddress::Any)) {
        qDebug() << "TCP Server listening on port" << m_tcpServer->serverPort();
    } else {
        qDebug() << "Failed to start TCP Server:" << m_tcpServer->errorString();
    }

    // Setup UDP Socket for discovering other OS instances on LAN
    m_udpSocket = new QUdpSocket(this);

    // Setup timers
    m_discoveryTimer = new QTimer(this);
    connect(m_discoveryTimer, &QTimer::timeout, this, &Backend::sendDiscoveryBroadcast);

    m_peerCheckTimer = new QTimer(this);
    connect(m_peerCheckTimer, &QTimer::timeout, this, &Backend::checkInactivePeers);

    // Trigger state configurations based on loaded settings
    if (m_clipboardSync) {
        connect(QGuiApplication::clipboard(), &QClipboard::dataChanged, this, &Backend::onClipboardChanged);
    }
    if (m_lanSync) {
        // Force-refresh LAN setup
        m_lanSync = false;
        setLanSync(true);
    }

    // Ensure storage file exists and has a starting header
    QFile file(m_sharedPath);
    if (!file.exists()) {
        clearHistory();
    }

    cleanupOldTrash();
}

Backend::~Backend() {
    saveSettings();
}

void Backend::loadSettings() {
    QSettings settings("OSDropZone", "CrossOS");
    m_sharedPath = settings.value("sharedPath", QDir::homePath() + "/shared_notes.txt").toString();
    m_trashPath = QFileInfo(m_sharedPath).absoluteDir().absolutePath() + "/karchara_notes.txt";
    m_clipboardSync = settings.value("clipboardSync", false).toBool();
    m_lanSync = settings.value("lanSync", false).toBool();
}

void Backend::saveSettings() {
    QSettings settings("OSDropZone", "CrossOS");
    settings.setValue("sharedPath", m_sharedPath);
    settings.setValue("clipboardSync", m_clipboardSync);
    settings.setValue("lanSync", m_lanSync);
}

QString Backend::getCurrentOS() {
#ifdef _WIN32
    return "Windows";
#elif __APPLE__
    return "macOS";
#elif __linux__
    return "Arch Linux";
#else
    return "Linux/Unix";
#endif
}

QString Backend::getDeviceName() {
    return QHostInfo::localHostName();
}

void Backend::setSharedPath(const QString &path) {
    QString cleanPath = path;
    if (cleanPath.startsWith("file://")) {
        cleanPath = QUrl(cleanPath).toLocalFile();
    }
    if (m_sharedPath != cleanPath) {
        m_sharedPath = cleanPath;
        m_trashPath = QFileInfo(m_sharedPath).absoluteDir().absolutePath() + "/karchara_notes.txt";
        saveSettings();
        emit sharedPathChanged();
        emit historyTextChanged();
    }
}

QString Backend::historyText() {
    return loadHistory();
}

void Backend::setClipboardSync(bool enable) {
    if (m_clipboardSync != enable) {
        m_clipboardSync = enable;
        saveSettings();

        if (m_clipboardSync) {
            connect(QGuiApplication::clipboard(), &QClipboard::dataChanged, this, &Backend::onClipboardChanged);
            // Sync current clipboard right away
            onClipboardChanged();
        } else {
            disconnect(QGuiApplication::clipboard(), &QClipboard::dataChanged, this, &Backend::onClipboardChanged);
        }

        emit clipboardSyncChanged();
    }
}

void Backend::setLanSync(bool enable) {
    if (m_lanSync != enable) {
        m_lanSync = enable;
        saveSettings();

        if (m_lanSync) {
            // Bind UDP to listen for broadcasts
            if (m_udpSocket->bind(m_discoveryPort, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint)) {
                connect(m_udpSocket, &QUdpSocket::readyRead, this, &Backend::processPendingDatagrams);
                qDebug() << "Bound UDP discovery socket to port" << m_discoveryPort;
            } else {
                qDebug() << "Failed to bind UDP socket:" << m_udpSocket->errorString();
            }

            m_discoveryTimer->start(2000); // Broadcast every 2 seconds
            m_peerCheckTimer->start(3000);  // Check inactive peers every 3 seconds
            sendDiscoveryBroadcast();       // Broadcast immediately
        } else {
            m_discoveryTimer->stop();
            m_peerCheckTimer->stop();
            m_udpSocket->close();
            disconnect(m_udpSocket, &QUdpSocket::readyRead, this, &Backend::processPendingDatagrams);
            m_peers.clear();
            emit peerListChanged();
        }

        emit lanSyncChanged();
    }
}

QStringList Backend::peerList() const {
    QStringList list;
    for (const auto &peer : m_peers) {
        list.append(QString("[%1] %2 (%3:%4)")
                    .arg(peer.os)
                    .arg(peer.deviceName)
                    .arg(peer.ip)
                    .arg(peer.tcpPort));
    }
    if (list.isEmpty()) {
        list.append("No active network peers found.");
    }
    return list;
}

void Backend::saveNote(const QString &text, const QString &type) {
    if (text.trimmed().isEmpty()) return;

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString header = QString("[%1 | OS: %2 | Type: %3]")
                         .arg(timestamp)
                         .arg(getCurrentOS())
                         .arg(type);

    appendLogToFile(header, text);

    if (m_lanSync) {
        broadcastNote(text, type);
    }

    emit historyTextChanged();
}

void Backend::saveFileDrop(const QUrl &url) {
    QString localPath = url.toLocalFile();
    if (localPath.isEmpty()) return;

    QFileInfo fileInfo(localPath);
    if (!fileInfo.exists()) return;

    // Create a drops directory adjacent to the shared notes text file
    QDir sharedDir = QFileInfo(m_sharedPath).absoluteDir();
    QString dropsPath = sharedDir.absolutePath() + "/drops";
    QDir().mkpath(dropsPath);

    QString destPath = dropsPath + "/" + fileInfo.fileName();

    if (QFile::exists(destPath)) {
        QFile::remove(destPath);
    }

    if (QFile::copy(localPath, destPath)) {
        saveNote(QString("File Shared! Check your drops directory.\nName: %1\nCopied to: %2")
                 .arg(fileInfo.fileName())
                 .arg(destPath), "File Drop");
    } else {
        saveNote(QString("File Referenced (Copy failed/same path).\nName: %1\nLocation: %2")
                 .arg(fileInfo.fileName())
                 .arg(localPath), "File Link");
    }
}

QString Backend::loadHistory() {
    QFile file(m_sharedPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "┌────────────────────────────────────────────────────────────┐\n"
               "│              Welcome to OS DropZone Terminal               │\n"
               "├────────────────────────────────────────────────────────────┤\n"
               "│ Log file empty or missing. Drop your first note here!      │\n"
               "└────────────────────────────────────────────────────────────┘\n";
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    if (content.trimmed().isEmpty()) {
        return "History file is empty.";
    }
    return content;
}

void Backend::clearHistory() {
    QFile file(m_sharedPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "==============================================================\n"
            << "             OS DROPZONE LOG FILE INITIALIZED                 \n"
            << "==============================================================\n"
            << "Created: " << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "\n"
            << "OS Host: " << getCurrentOS() << " (" << getDeviceName() << ")\n"
            << "Path:    " << m_sharedPath << "\n"
            << "==============================================================\n\n";
        file.close();
    }
    emit historyTextChanged();
}

void Backend::copyToClipboard(const QString &text) {
    if (text.isEmpty()) return;
    
    // Prevent feedback loops by updating tracking string
    m_lastClipboardText = text;

    // Disconnect clipboard sync while we modify it
    if (m_clipboardSync) {
        disconnect(QGuiApplication::clipboard(), &QClipboard::dataChanged, this, &Backend::onClipboardChanged);
    }

    QGuiApplication::clipboard()->setText(text);

    if (m_clipboardSync) {
        // Allow a small delay or reconnect immediately
        connect(QGuiApplication::clipboard(), &QClipboard::dataChanged, this, &Backend::onClipboardChanged);
    }
}

void Backend::openSharedFolder() {
    QString folderPath = QFileInfo(m_sharedPath).absolutePath();
    QDesktopServices::openUrl(QUrl::fromLocalFile(folderPath));
}

void Backend::onClipboardChanged() {
    if (!m_clipboardSync) return;

    QString text = QGuiApplication::clipboard()->text();
    if (text.trimmed().isEmpty() || text == m_lastClipboardText) return;

    m_lastClipboardText = text;
    saveNote(text, "Clipboard");
}

void Backend::sendDiscoveryBroadcast() {
    if (!m_lanSync || !m_tcpServer->isListening()) return;

    // Payload: CROSS_OS_DISCOVER:<TCP_PORT>:<OS>:<DEVICE_NAME>
    QString message = QString("CROSS_OS_DISCOVER:%1:%2:%3")
                          .arg(m_tcpServer->serverPort())
                          .arg(getCurrentOS())
                          .arg(getDeviceName());

    QByteArray datagram = message.toUtf8();
    m_udpSocket->writeDatagram(datagram, QHostAddress::Broadcast, m_discoveryPort);
}

void Backend::processPendingDatagrams() {
    while (m_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        QHostAddress senderIp;
        m_udpSocket->readDatagram(datagram.data(), datagram.size(), &senderIp);

        QString dataStr = QString::fromUtf8(datagram);
        QStringList parts = dataStr.split(":");

        if (parts.size() >= 4 && parts[0] == "CROSS_OS_DISCOVER") {
            quint16 tcpPort = parts[1].toUShort();
            QString os = parts[2];
            QString deviceName = parts[3];
            QString ipStr = senderIp.toString();

            // Ignore our own broadcasts
            if (deviceName == getDeviceName() && tcpPort == m_tcpServer->serverPort()) {
                continue;
            }

            // Clean IPv6-mapped IPv4 addresses (e.g. ::ffff:192.168.1.10)
            if (ipStr.startsWith("::ffff:")) {
                ipStr = ipStr.mid(7);
            }

            bool found = false;
            for (auto &peer : m_peers) {
                if (peer.deviceName == deviceName && peer.tcpPort == tcpPort) {
                    peer.lastSeen = QDateTime::currentDateTime();
                    peer.ip = ipStr;
                    found = true;
                    break;
                }
            }

            if (!found) {
                PeerInfo newPeer = { ipStr, tcpPort, os, deviceName, QDateTime::currentDateTime() };
                m_peers.append(newPeer);
                qDebug() << "Discovered new peer:" << deviceName << "at" << ipStr << ":" << tcpPort;
                emit peerListChanged();
            }
        }
    }
}

void Backend::handleNewConnection() {
    QTcpSocket *clientSocket = m_tcpServer->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, this, &Backend::readClientData);
    connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);
}

void Backend::readClientData() {
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if (!socket) return;

    QByteArray data = socket->readAll();
    QString dataStr = QString::fromUtf8(data);
    QStringList lines = dataStr.split("\n");

    if (lines.size() >= 4 && lines[0] == "NOTE") {
        QString os = lines[1];
        QString type = lines[2];
        
        // Reassemble the content if it contains newlines
        QString content;
        for (int i = 3; i < lines.size(); ++i) {
            content += lines[i];
            if (i < lines.size() - 1) content += "\n";
        }

        QString ipAddress = socket->peerAddress().toString();
        if (ipAddress.startsWith("::ffff:")) {
            ipAddress = ipAddress.mid(7);
        }

        QString senderLabel = QString("Network Peer (%1 @ %2)").arg(socket->peerName()).arg(ipAddress);
        
        // Save note and refresh UI
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        QString header = QString("[%1 | OS: %2 | Type: %3 (via LAN from %4)]")
                             .arg(timestamp)
                             .arg(os)
                             .arg(type)
                             .arg(ipAddress);

        appendLogToFile(header, content);
        emit historyTextChanged();
        emit noteReceived(ipAddress, content);

        // If it was clipboard content and sync is enabled, sync to local clipboard
        if (type == "Clipboard" && m_clipboardSync) {
            copyToClipboard(content);
        }
    }
}

void Backend::checkInactivePeers() {
    QDateTime cutoff = QDateTime::currentDateTime().addSecs(-6); // 6 seconds threshold
    bool changed = false;

    for (int i = m_peers.size() - 1; i >= 0; --i) {
        if (m_peers[i].lastSeen < cutoff) {
            qDebug() << "Peer inactive, removing:" << m_peers[i].deviceName;
            m_peers.removeAt(i);
            changed = true;
        }
    }

    if (changed) {
        emit peerListChanged();
    }
}

void Backend::appendLogToFile(const QString &header, const QString &body) {
    QFile file(m_sharedPath);
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << "==============================================================\n"
            << header << "\n"
            << "==============================================================\n"
            << body << "\n"
            << "--------------------------------------------------------------\n\n";
        file.close();
    }
}

void Backend::broadcastNote(const QString &text, const QString &type) {
    for (const auto &peer : m_peers) {
        QTcpSocket socket;
        socket.connectToHost(peer.ip, peer.tcpPort);
        if (socket.waitForConnected(1000)) {
            // Write formatted note
            QString payload = QString("NOTE\n%1\n%2\n%3")
                                  .arg(getCurrentOS())
                                  .arg(type)
                                  .arg(text);
            socket.write(payload.toUtf8());
            socket.waitForBytesWritten(1000);
            socket.disconnectFromHost();
        } else {
            qDebug() << "Failed to connect to peer for broadcasting note:" << peer.deviceName;
        }
    }
}

QString Backend::getLocalIPAddress() {
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList[i] != QHostAddress::LocalHost && ipAddressesList[i].toIPv4Address()) {
            ipAddress = ipAddressesList[i].toString();
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    return ipAddress;
}

#include <algorithm>

// Comparator to sort pinned notes first, then by timestamp descending
bool compareNotes(const NoteEntry &a, const NoteEntry &b) {
    if (a.pinned != b.pinned) {
        return a.pinned; // true (pinned) comes before false
    }
    return a.timestamp > b.timestamp; // newest first
}

// Comparator to sort trash notes by deletion date (newest deleted first)
bool compareTrash(const NoteEntry &a, const NoteEntry &b) {
    return a.deletedAt > b.deletedAt; // newest deleted first
}

QVariantList Backend::notes() {
    QList<NoteEntry> notesList = parseNotesFile(m_sharedPath);
    std::sort(notesList.begin(), notesList.end(), compareNotes);
    
    QVariantList list;
    for (int i = 0; i < notesList.size(); ++i) {
        const auto &note = notesList[i];
        QVariantMap map;
        map["index"] = i;
        map["timestamp"] = note.timestamp;
        map["os"] = note.os;
        map["type"] = note.type;
        map["pinned"] = note.pinned;
        map["body"] = note.body;
        list.append(map);
    }
    return list;
}

void Backend::deleteNote(int index) {
    QList<NoteEntry> notesList = parseNotesFile(m_sharedPath);
    std::sort(notesList.begin(), notesList.end(), compareNotes);
    
    if (index >= 0 && index < notesList.size()) {
        NoteEntry deletedNote = notesList[index];
        deletedNote.deletedAt = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        
        // Append to trash notes file
        QList<NoteEntry> trashList = parseNotesFile(m_trashPath);
        trashList.append(deletedNote);
        writeNotesToFile(m_trashPath, trashList);
        
        // Remove from active notes file
        notesList.removeAt(index);
        writeNotesToFile(m_sharedPath, notesList);
        
        emit historyTextChanged();
    }
}

void Backend::togglePin(int index) {
    QList<NoteEntry> notesList = parseNotesFile(m_sharedPath);
    std::sort(notesList.begin(), notesList.end(), compareNotes);
    
    if (index >= 0 && index < notesList.size()) {
        notesList[index].pinned = !notesList[index].pinned;
        writeNotesToFile(m_sharedPath, notesList);
        emit historyTextChanged();
    }
}

void Backend::restoreNote(int index) {
    QList<NoteEntry> trashList = parseNotesFile(m_trashPath);
    std::sort(trashList.begin(), trashList.end(), compareTrash);
    
    if (index >= 0 && index < trashList.size()) {
        NoteEntry restoredNote = trashList[index];
        restoredNote.deletedAt = ""; // reset deleted status
        
        // Append to active notes file
        QList<NoteEntry> activeList = parseNotesFile(m_sharedPath);
        activeList.append(restoredNote);
        writeNotesToFile(m_sharedPath, activeList);
        
        // Remove from trash notes file
        trashList.removeAt(index);
        writeNotesToFile(m_trashPath, trashList);
        
        emit historyTextChanged();
    }
}

void Backend::deleteNoteForever(int index) {
    QList<NoteEntry> trashList = parseNotesFile(m_trashPath);
    std::sort(trashList.begin(), trashList.end(), compareTrash);
    
    if (index >= 0 && index < trashList.size()) {
        trashList.removeAt(index);
        writeNotesToFile(m_trashPath, trashList);
        emit historyTextChanged();
    }
}

QVariantList Backend::trashNotes() {
    cleanupOldTrash();
    QList<NoteEntry> trashList = parseNotesFile(m_trashPath);
    std::sort(trashList.begin(), trashList.end(), compareTrash);
    
    QVariantList list;
    for (int i = 0; i < trashList.size(); ++i) {
        const auto &note = trashList[i];
        QVariantMap map;
        map["index"] = i;
        map["timestamp"] = note.timestamp;
        map["os"] = note.os;
        map["type"] = note.type;
        map["deletedAt"] = note.deletedAt;
        map["body"] = note.body;
        list.append(map);
    }
    return list;
}

QList<NoteEntry> Backend::parseNotesFile(const QString &filePath) {
    QList<NoteEntry> notes;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return notes;
    }
    
    QTextStream in(&file);
    bool inBody = false;
    NoteEntry currentEntry;
    
    while (!in.atEnd()) {
        QString line = in.readLine();
        QString trimmed = line.trimmed();
        
        if (trimmed.startsWith("[") && trimmed.endsWith("]")) {
            // Found a header!
            QString header = trimmed.mid(1, trimmed.length() - 2);
            QStringList headerParts = header.split(" | ");
            if (headerParts.size() >= 3) {
                currentEntry.timestamp = headerParts[0];
                
                if (headerParts[1].startsWith("OS: ")) {
                    currentEntry.os = headerParts[1].mid(4);
                } else {
                    currentEntry.os = headerParts[1];
                }
                
                if (headerParts[2].startsWith("Type: ")) {
                    currentEntry.type = headerParts[2].mid(6);
                } else {
                    currentEntry.type = headerParts[2];
                }
                
                currentEntry.pinned = false;
                currentEntry.deletedAt = "";
                
                if (headerParts.size() >= 4) {
                    for (int i = 3; i < headerParts.size(); ++i) {
                        if (headerParts[i] == "Pinned") {
                            currentEntry.pinned = true;
                        } else if (headerParts[i].startsWith("DeletedAt: ")) {
                            currentEntry.deletedAt = headerParts[i].mid(11);
                        }
                    }
                }
                
                currentEntry.body = "";
                inBody = true;
            }
        } else if (trimmed == "--------------------------------------------------------------") {
            if (inBody) {
                currentEntry.body = currentEntry.body.trimmed();
                notes.append(currentEntry);
                inBody = false;
            }
        } else {
            if (inBody) {
                // Skip secondary section separator line between header and body if it exists
                if (trimmed == "==============================================================") {
                    continue;
                }
                currentEntry.body += line + "\n";
            }
        }
    }
    
    file.close();
    return notes;
}

void Backend::writeNotesToFile(const QString &filePath, const QList<NoteEntry> &notes) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }
    
    QTextStream out(&file);
    out << "==============================================================\n"
        << "             OS DROPZONE LOG FILE INITIALIZED                 \n"
        << "==============================================================\n"
        << "Updated: " << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "\n"
        << "OS Host: " << Backend::getCurrentOS() << " (" << Backend::getDeviceName() << ")\n"
        << "Path:    " << filePath << "\n"
        << "==============================================================\n\n";
        
    for (const auto &note : notes) {
        QString header = QString("[%1 | OS: %2 | Type: %3]")
                             .arg(note.timestamp)
                             .arg(note.os)
                             .arg(note.type);
        if (note.pinned) {
            header.chop(1); // remove ]
            header += " | Pinned]";
        }
        if (!note.deletedAt.isEmpty()) {
            header.chop(1); // remove ]
            header += " | DeletedAt: " + note.deletedAt + "]";
        }
        
        out << "==============================================================\n"
            << header << "\n"
            << "==============================================================\n"
            << note.body << "\n"
            << "--------------------------------------------------------------\n\n";
    }
    
    file.close();
}

void Backend::cleanupOldTrash() {
    QList<NoteEntry> trashList = parseNotesFile(m_trashPath);
    QList<NoteEntry> remainingTrash;
    QDateTime now = QDateTime::currentDateTime();
    bool changed = false;
    
    for (const auto &note : trashList) {
        if (!note.deletedAt.isEmpty()) {
            QDateTime deletedTime = QDateTime::fromString(note.deletedAt, "yyyy-MM-dd HH:mm:ss");
            if (deletedTime.isValid()) {
                if (deletedTime.daysTo(now) >= 7) {
                    changed = true;
                    qDebug() << "Permanently deleting trash note older than 1 week from:" << note.timestamp;
                    continue; // skip, permanently deleting it
                }
            }
        }
        remainingTrash.append(note);
    }
    
    if (changed) {
        writeNotesToFile(m_trashPath, remainingTrash);
    }
}