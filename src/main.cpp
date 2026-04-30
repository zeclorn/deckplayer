#include "AppState.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationName(QStringLiteral("steamdeckmediaplayer"));
    app.setApplicationName(QStringLiteral("steamdeckmediaplayer"));

    QQmlApplicationEngine engine;
    AppState appState;
    engine.rootContext()->setContextProperty(QStringLiteral("appState"), &appState);

    const QUrl url(QStringLiteral("qrc:/qt/qml/SteamDeckMediaPlayer/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
