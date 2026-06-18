#pragma once
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QDateTime>

using namespace std;

class Backend : public QObject {
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr) : QObject(parent) {}

    // Function to detect the current OS automatically
    QString getCurrentOS() {
        #ifdef _WIN32
            return "Windows";
        #elif __linux__
            return "Arch Linux";
        #else
            return "Unknown System";
        #endif
    }

    // This path should be your shared drive!
    QString sharedPath = QDir::homePath() + "/shared_notes.txt"; 

public slots:
    // Slot to save the note with a timestamp and OS tag
    void saveNote(const QString &text) {
        QFile file(sharedPath);
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm");
            
            out << "[" << timestamp << " | Saved from " << getCurrentOS() << "]\n";
            out << text << "\n";
            out << "----------------------------------------\n";
            file.close();
        }
    }

    // New slot: QML calls this to read the past history
    QString loadHistory() {
        QFile file(sharedPath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return "No history found yet. Drop your first note!";
        }
        QTextStream in(&file);
        return in.readAll();
    }
};