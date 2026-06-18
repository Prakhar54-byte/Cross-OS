#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "backend.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    
    // Create the backend and expose it to the UI
    Backend backend;
    engine.rootContext()->setContextProperty("Backend", &backend);

    // The modernized Qt6 string format (Removed the _qs)
    const QUrl url("qrc:/Main/main.qml");
    
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
        
    engine.load(url);

    return app.exec();
}