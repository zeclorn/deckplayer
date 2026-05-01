#pragma once

#include <QByteArray>
#include <QList>
#include <QQuickFramebufferObject>
#include <QString>

struct mpv_handle;
struct mpv_render_context;

class QOpenGLContext;
class MpvRenderer;

class MpvVideoItem : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(QString mediaPath READ mediaPath WRITE setMediaPath NOTIFY mediaPathChanged)
    Q_PROPERTY(qint64 startPositionMs READ startPositionMs WRITE setStartPositionMs NOTIFY startPositionMsChanged)
    Q_PROPERTY(int requestId READ requestId WRITE setRequestId NOTIFY requestIdChanged)
    Q_PROPERTY(bool paused READ paused NOTIFY pausedChanged)
    Q_PROPERTY(bool loaded READ loaded NOTIFY loadedChanged)
    Q_PROPERTY(qint64 positionMs READ positionMs NOTIFY positionChanged)
    Q_PROPERTY(qint64 durationMs READ durationMs NOTIFY durationChanged)
    Q_PROPERTY(QString subtitleTrackLabel READ subtitleTrackLabel NOTIFY subtitleTrackLabelChanged)
    Q_PROPERTY(QString audioTrackLabel READ audioTrackLabel NOTIFY audioTrackLabelChanged)

public:
    explicit MpvVideoItem(QQuickItem *parent = nullptr);
    ~MpvVideoItem() override;

    Renderer *createRenderer() const override;

    QString mediaPath() const;
    void setMediaPath(const QString &mediaPath);

    qint64 startPositionMs() const;
    void setStartPositionMs(qint64 startPositionMs);

    int requestId() const;
    void setRequestId(int requestId);

    bool paused() const;
    bool loaded() const;
    qint64 positionMs() const;
    qint64 durationMs() const;
    QString subtitleTrackLabel() const;
    QString audioTrackLabel() const;

    Q_INVOKABLE void togglePause();
    Q_INVOKABLE void seekForward();
    Q_INVOKABLE void seekBackward();
    Q_INVOKABLE void cycleSubtitles();
    Q_INVOKABLE void cycleAudioTracks();
    Q_INVOKABLE void stop();

signals:
    void mediaPathChanged();
    void startPositionMsChanged();
    void requestIdChanged();
    void pausedChanged();
    void loadedChanged();
    void positionChanged();
    void durationChanged();
    void subtitleTrackLabelChanged();
    void audioTrackLabelChanged();
    void playbackFinished();
    void playbackStopped();

private slots:
    void processMpvEvents();

private:
    friend class MpvRenderer;

    void maybeLoadFile();
    void initializeMpv();
    void initializeRenderContext(QOpenGLContext *context);
    void destroyRenderContext();
    void setPaused(bool paused);
    void setLoaded(bool loaded);
    void setPositionMs(qint64 positionMs);
    void setDurationMs(qint64 durationMs);
    void refreshTrackLabels();
    void setSubtitleTrackLabel(const QString &subtitleTrackLabel);
    void setAudioTrackLabel(const QString &audioTrackLabel);
    QString selectedTrackLabel(const char *type, const QString &enabledPrefix, const QString &disabledLabel) const;
    QString propertyString(const QByteArray &name) const;
    void command(const QList<QByteArray> &arguments);

    static void onMpvWakeup(void *context);
    static void onMpvUpdate(void *context);
    static void *getProcAddress(void *context, const char *name);

    mpv_handle *m_mpv = nullptr;
    mpv_render_context *m_renderContext = nullptr;
    QString m_mediaPath;
    qint64 m_startPositionMs = 0;
    int m_requestId = 0;
    int m_loadedRequestId = -1;
    qint64 m_pendingSeekPositionMs = 0;
    bool m_paused = false;
    bool m_loaded = false;
    qint64 m_positionMs = 0;
    qint64 m_durationMs = 0;
    QString m_subtitleTrackLabel = QStringLiteral("Subtitles Off");
    QString m_audioTrackLabel;
};
