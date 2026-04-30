#pragma once

#include "FileBrowserModel.h"

#include <QObject>
#include <QSettings>

class AppState final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(FileBrowserModel *browserModel READ browserModel CONSTANT)
    Q_PROPERTY(bool playerVisible READ playerVisible NOTIFY playerVisibleChanged)
    Q_PROPERTY(bool resumePromptVisible READ resumePromptVisible NOTIFY resumePromptVisibleChanged)
    Q_PROPERTY(QString currentMediaPath READ currentMediaPath NOTIFY currentMediaChanged)
    Q_PROPERTY(QString currentMediaTitle READ currentMediaTitle NOTIFY currentMediaChanged)
    Q_PROPERTY(qint64 playbackStartPositionMs READ playbackStartPositionMs NOTIFY currentMediaChanged)
    Q_PROPERTY(int playbackRequestId READ playbackRequestId NOTIFY currentMediaChanged)
    Q_PROPERTY(QString pendingMediaTitle READ pendingMediaTitle NOTIFY resumePromptVisibleChanged)
    Q_PROPERTY(qint64 pendingResumePositionMs READ pendingResumePositionMs NOTIFY resumePromptVisibleChanged)
    Q_PROPERTY(bool showHidden READ showHidden WRITE setShowHidden NOTIFY showHiddenChanged)

public:
    explicit AppState(QObject *parent = nullptr);

    FileBrowserModel *browserModel();
    bool playerVisible() const;
    bool resumePromptVisible() const;
    QString currentMediaPath() const;
    QString currentMediaTitle() const;
    qint64 playbackStartPositionMs() const;
    int playbackRequestId() const;
    QString pendingMediaTitle() const;
    qint64 pendingResumePositionMs() const;
    bool showHidden() const;

    Q_INVOKABLE void openEntry(const QString &path, bool isDirectory, bool isPlayable);
    Q_INVOKABLE void goBack();
    Q_INVOKABLE void navigateUp();
    Q_INVOKABLE void closePlayer();
    Q_INVOKABLE void chooseResume();
    Q_INVOKABLE void chooseStartOver();
    Q_INVOKABLE void savePlaybackPosition(qint64 positionMs);
    Q_INVOKABLE void clearResume(const QString &path);
    Q_INVOKABLE QString formatDuration(qint64 durationMs) const;

public slots:
    void setShowHidden(bool showHidden);

signals:
    void playerVisibleChanged();
    void resumePromptVisibleChanged();
    void currentMediaChanged();
    void showHiddenChanged();

private:
    void loadSettings();
    void persistSettings();
    void beginPlayback(const QString &path, qint64 startPositionMs);
    qint64 savedResumeForPath(const QString &path) const;

    FileBrowserModel m_browserModel;
    QSettings m_settings;
    QString m_currentMediaPath;
    QString m_currentMediaTitle;
    qint64 m_playbackStartPositionMs = 0;
    int m_playbackRequestId = 0;
    QString m_pendingMediaPath;
    QString m_pendingMediaTitle;
    qint64 m_pendingResumePositionMs = 0;
    bool m_playerVisible = false;
    bool m_resumePromptVisible = false;
};
