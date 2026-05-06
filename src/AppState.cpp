#include "AppState.h"

#include <QDir>
#include <QFileInfo>

namespace {

QString defaultVideosPath()
{
    const QString configuredPath = QDir::home().filePath(QStringLiteral("Videos"));
    QFileInfo info(configuredPath);
    if (info.exists() && info.isDir()) {
        return info.absoluteFilePath();
    }

    return QDir::homePath();
}

}

AppState::AppState(QObject *parent)
    : QObject(parent)
    , m_settings(QStringLiteral("steamdeckmediaplayer"), QStringLiteral("steamdeckmediaplayer"))
{
    loadSettings();
}

FileBrowserModel *AppState::browserModel()
{
    return &m_browserModel;
}

bool AppState::playerVisible() const
{
    return m_playerVisible;
}

QString AppState::currentMediaPath() const
{
    return m_currentMediaPath;
}

QString AppState::currentMediaTitle() const
{
    return m_currentMediaTitle;
}

qint64 AppState::playbackStartPositionMs() const
{
    return m_playbackStartPositionMs;
}

int AppState::playbackRequestId() const
{
    return m_playbackRequestId;
}

bool AppState::showHidden() const
{
    return m_browserModel.showHidden();
}

void AppState::openEntry(const QString &path, bool isDirectory, bool isPlayable)
{
    if (isDirectory) {
        m_browserModel.setCurrentPath(path);
        persistSettings();
        return;
    }

    if (!isPlayable) {
        return;
    }

    beginPlayback(path, loadPlaybackPosition(path));
}

void AppState::goBack()
{
    if (m_playerVisible) {
        closePlayer();
        return;
    }

    m_browserModel.goUp();
    persistSettings();
}

void AppState::navigateUp()
{
    m_browserModel.goUp();
    persistSettings();
}

void AppState::closePlayer()
{
    if (!m_playerVisible) {
        return;
    }

    m_playerVisible = false;
    emit playerVisibleChanged();
}

QString AppState::formatDuration(qint64 durationMs) const
{
    const qint64 totalSeconds = durationMs / 1000;
    const qint64 hours = totalSeconds / 3600;
    const qint64 minutes = (totalSeconds % 3600) / 60;
    const qint64 seconds = totalSeconds % 60;

    if (hours > 0) {
        return QStringLiteral("%1:%2:%3")
            .arg(hours)
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));
    }

    return QStringLiteral("%1:%2")
        .arg(minutes)
        .arg(seconds, 2, 10, QChar('0'));
}

void AppState::setShowHidden(bool showHidden)
{
    if (m_browserModel.showHidden() == showHidden) {
        return;
    }

    m_browserModel.setShowHidden(showHidden);
    persistSettings();
    emit showHiddenChanged();
}

void AppState::loadSettings()
{
    const QString lastPath = m_settings.value(QStringLiteral("browser/lastPath"), defaultVideosPath()).toString();
    const bool showHidden = m_settings.value(QStringLiteral("browser/showHidden"), false).toBool();

    m_browserModel.setShowHidden(showHidden);
    m_browserModel.setCurrentPath(lastPath);
}

void AppState::persistSettings()
{
    m_settings.setValue(QStringLiteral("browser/lastPath"), m_browserModel.currentPath());
    m_settings.setValue(QStringLiteral("browser/showHidden"), m_browserModel.showHidden());
}

void AppState::savePlaybackPosition(const QString &path, qint64 positionMs)
{
    if (positionMs > 0) {
        m_settings.setValue(positionSettingsKey(path), positionMs);
    }
}

void AppState::clearPlaybackPosition(const QString &path)
{
    m_settings.remove(positionSettingsKey(path));
}

qint64 AppState::loadPlaybackPosition(const QString &path) const
{
    return m_settings.value(positionSettingsKey(path), 0LL).toLongLong();
}

QString AppState::positionSettingsKey(const QString &path) const
{
    return QStringLiteral("playback/positions/") + QString::fromUtf8(path.toUtf8().toBase64());
}

void AppState::beginPlayback(const QString &path, qint64 startPositionMs)
{
    const QFileInfo fileInfo(path);

    m_currentMediaPath = path;
    m_currentMediaTitle = fileInfo.fileName();
    m_playbackStartPositionMs = startPositionMs;
    ++m_playbackRequestId;
    m_playerVisible = true;
    emit currentMediaChanged();
    emit playerVisibleChanged();
}
