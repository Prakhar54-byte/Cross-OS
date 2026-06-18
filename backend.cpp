#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDebug>

class Backend : public QObject {
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr) : QObject(parent) {}

public slots:
    // This slot is callable directly from the QML UI
    void saveNote(const QString &text) {
        // Path changes based on OS, you can use preprocessor directives #ifdef _WIN32
        QString sharedPath = "/mnt/shared_workspace/notes.txt"; 
        
        QFile file(sharedPath);
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << text << "\n---\n";
            file.close();
            qDebug() << "Saved to dual-boot drive!";
        }
    }
};