#include "AppState.h"

#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringList>

namespace {

constexpr qint64 kMinimumResumePromptMs = 30 * 1000;

QString defaultVideosPath()
{
    const QString configuredPath = QDir::home().filePath(QStringLiteral("Videos"));
    QFileInfo info(configuredPath);
    if (info.exists() && info.isDir()) {
        return info.absoluteFilePath();
    }

    return QDir::homePath();
}

QString resumeStorageKey()
{
    return QStringLiteral("playback/resumePositions");
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

bool AppState::resumePromptVisible() const
{
    return m_resumePromptVisible;
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

QString AppState::pendingMediaTitle() const
{
    return m_pendingMediaTitle;
}

qint64 AppState::pendingResumePositionMs() const
{
    return m_pendingResumePositionMs;
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

    const qint64 resumePosition = savedResumeForPath(path);
    if (resumePosition >= kMinimumResumePromptMs) {
        m_pendingMediaPath = path;
        m_pendingMediaTitle = QFileInfo(path).fileName();
        m_pendingResumePositionMs = resumePosition;
        m_resumePromptVisible = true;
        emit resumePromptVisibleChanged();
        return;
    }

    beginPlayback(path, 0);
}

void AppState::goBack()
{
    if (m_resumePromptVisible) {
        m_resumePromptVisible = false;
        m_pendingMediaPath.clear();
        m_pendingMediaTitle.clear();
        m_pendingResumePositionMs = 0;
        emit resumePromptVisibleChanged();
        return;
    }

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

void AppState::chooseResume()
{
    if (!m_resumePromptVisible) {
        return;
    }

    const QString pendingPath = m_pendingMediaPath;
    const qint64 pendingPosition = m_pendingResumePositionMs;

    m_resumePromptVisible = false;
    m_pendingMediaPath.clear();
    m_pendingMediaTitle.clear();
    m_pendingResumePositionMs = 0;
    emit resumePromptVisibleChanged();

    beginPlayback(pendingPath, pendingPosition);
}

void AppState::chooseStartOver()
{
    if (!m_resumePromptVisible) {
        return;
    }

    const QString pendingPath = m_pendingMediaPath;
    clearResume(pendingPath);

    m_resumePromptVisible = false;
    m_pendingMediaPath.clear();
    m_pendingMediaTitle.clear();
    m_pendingResumePositionMs = 0;
    emit resumePromptVisibleChanged();

    beginPlayback(pendingPath, 0);
}

void AppState::savePlaybackPosition(qint64 positionMs)
{
    if (m_currentMediaPath.isEmpty()) {
        return;
    }

    QJsonObject object = QJsonDocument::fromJson(m_settings.value(resumeStorageKey()).toByteArray()).object();
    object.insert(m_currentMediaPath, QString::number(positionMs));
    m_settings.setValue(resumeStorageKey(), QJsonDocument(object).toJson(QJsonDocument::Compact));
}

void AppState::clearResume(const QString &path)
{
    if (path.isEmpty()) {
        return;
    }

    QJsonObject object = QJsonDocument::fromJson(m_settings.value(resumeStorageKey()).toByteArray()).object();
    object.remove(path);
    m_settings.setValue(resumeStorageKey(), QJsonDocument(object).toJson(QJsonDocument::Compact));
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

qint64 AppState::savedResumeForPath(const QString &path) const
{
    const QJsonObject object = QJsonDocument::fromJson(m_settings.value(resumeStorageKey()).toByteArray()).object();
    return object.value(path).toString().toLongLong();
}
