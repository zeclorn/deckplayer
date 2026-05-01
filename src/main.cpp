#include "AppState.h"
#include "MpvVideoItem.h"

#include <clocale>
#include <QGuiApplication>
#include <QLocale>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QSGRendererInterface>
#include <QUrl>
#include <QByteArray>
#include <qqml.h>

int main(int argc, char *argv[])
{
    qputenv("LC_NUMERIC", QByteArrayLiteral("C"));
    qputenv("LANG", QByteArrayLiteral("C"));
    qputenv("LC_ALL", QByteArrayLiteral("C"));
    std::setlocale(LC_ALL, "C");
    std::setlocale(LC_NUMERIC, "C");
    QLocale::setDefault(QLocale::c());

    QGuiApplication app(argc, argv);
    app.setOrganizationName(QStringLiteral("steamdeckmediaplayer"));
    app.setApplicationName(QStringLiteral("steamdeckmediaplayer"));

    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);

    qmlRegisterType<MpvVideoItem>("SteamDeckMediaPlayer", 1, 0, "MpvVideoItem");

    AppState appState;
    qmlRegisterSingletonInstance("SteamDeckMediaPlayer", 1, 0, "AppState", &appState);

    QQmlApplicationEngine engine;

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(QUrl(QStringLiteral("qrc:/SteamDeckMediaPlayer/qml/Main.qml")));

    return app.exec();
}
