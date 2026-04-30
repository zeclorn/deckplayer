#include "AppState.h"
#include "MpvVideoItem.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <qqml.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setOrganizationName(QStringLiteral("steamdeckmediaplayer"));
    app.setApplicationName(QStringLiteral("steamdeckmediaplayer"));

    qmlRegisterType<MpvVideoItem>("SteamDeckMediaPlayer", 1, 0, "MpvVideoItem");

    QQmlApplicationEngine engine;
    AppState appState;
    engine.rootContext()->setContextProperty(QStringLiteral("appState"), &appState);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule(QStringLiteral("SteamDeckMediaPlayer"), QStringLiteral("Main"));

    return app.exec();
}
