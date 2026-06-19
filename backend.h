#pragma once
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QDateTime>
#include <QUrl>
#include <QFileInfo>
#include <QSettings>
#include <QClipboard>
#include <QGuiApplication>
#include <QUdpSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QStorageInfo>

struct NoteEntry {
    QString timestamp;
    QString os;
    QString type;
    bool pinned = false;
    QString body;
    QString deletedAt;
};

class Backend : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString sharedPath READ sharedPath WRITE setSharedPath NOTIFY sharedPathChanged)
    Q_PROPERTY(QString historyText READ historyText NOTIFY historyTextChanged)
    Q_PROPERTY(bool clipboardSync READ clipboardSync WRITE setClipboardSync NOTIFY clipboardSyncChanged)
    Q_PROPERTY(bool lanSync READ lanSync WRITE setLanSync NOTIFY lanSyncChanged)
    Q_PROPERTY(QStringList peerList READ peerList NOTIFY peerListChanged)
    Q_PROPERTY(QString localIP READ localIP CONSTANT)
    Q_PROPERTY(QString currentOS READ currentOS CONSTANT)
    Q_PROPERTY(QString deviceName READ deviceName CONSTANT)
    Q_PROPERTY(QVariantList notes READ notes NOTIFY historyTextChanged)
    Q_PROPERTY(QVariantList trashNotes READ trashNotes NOTIFY historyTextChanged)
    Q_PROPERTY(QVariantList mountedDrives READ mountedDrives NOTIFY mountedDrivesChanged)
    Q_PROPERTY(QVariantList droppedFiles READ droppedFiles NOTIFY droppedFilesChanged)

public:
    explicit Backend(QObject *parent = nullptr);
    ~Backend();

    // Property Getters & Setters
    QString sharedPath() const { return m_sharedPath; }
    void setSharedPath(const QString &path);

    QString historyText();

    bool clipboardSync() const { return m_clipboardSync; }
    void setClipboardSync(bool enable);

    bool lanSync() const { return m_lanSync; }
    void setLanSync(bool enable);

    QStringList peerList() const;

    QString localIP() { return getLocalIPAddress(); }
    QString currentOS() { return getCurrentOS(); }
    QString deviceName() { return getDeviceName(); }
    QVariantList notes();
    QVariantList trashNotes();
    QVariantList mountedDrives();
    QVariantList droppedFiles();

    // Helper to detect current OS
    static QString getCurrentOS();
    static QString getDeviceName();

public slots:
    // Core functional slots called from QML
    void saveNote(const QString &text, const QString &type = "Note");
    void saveFileDrop(const QUrl &url);
    QString loadHistory();
    void clearHistory();
    void copyToClipboard(const QString &text);
    void openSharedFolder();
    void deleteNote(int index);
    void togglePin(int index);
    void restoreNote(int index);
    void deleteNoteForever(int index);
    void refreshMountedDrives();
    void refreshDroppedFiles();
    void openFileExternal(const QString &filePath);
    void deleteDroppedFile(const QString &fileName);
    QString dropsDirectory();

signals:
    void sharedPathChanged();
    void historyTextChanged();
    void clipboardSyncChanged();
    void lanSyncChanged();
    void peerListChanged();
    void noteReceived(const QString &sender, const QString &text);
    void mountedDrivesChanged();
    void droppedFilesChanged();

private slots:
    void onClipboardChanged();
    void sendDiscoveryBroadcast();
    void processPendingDatagrams();
    void handleNewConnection();
    void readClientData();
    void checkInactivePeers();

private:
    struct PeerInfo {
        QString ip;
        quint16 tcpPort;
        QString os;
        QString deviceName;
        QDateTime lastSeen;
    };

    void loadSettings();
    void saveSettings();
    void appendLogToFile(const QString &title, const QString &body);
    void broadcastNote(const QString &text, const QString &type);
    QString getLocalIPAddress();
    QList<NoteEntry> parseNotesFile(const QString &filePath);
    void writeNotesToFile(const QString &filePath, const QList<NoteEntry> &notes);
    void cleanupOldTrash();

    QString m_sharedPath;
    QString m_trashPath;
    QString m_historyCache;
    bool m_clipboardSync;
    bool m_lanSync;
    QString m_lastClipboardText;

    // Networking
    QUdpSocket *m_udpSocket = nullptr;
    QTcpServer *m_tcpServer = nullptr;
    QTimer *m_discoveryTimer = nullptr;
    QTimer *m_peerCheckTimer = nullptr;
    QList<PeerInfo> m_peers;
    quint16 m_discoveryPort = 40404;
};