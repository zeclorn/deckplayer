#include "MpvVideoItem.h"

#include <QMetaObject>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFunctions>
#include <QStringList>
#include <QVector>

#include <mpv/client.h>
#include <mpv/render_gl.h>

namespace {

constexpr int kSeekStepSeconds = 10;
constexpr int kVolumeStep = 5;

}

class MpvRenderer final : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions
{
public:
    explicit MpvRenderer(MpvVideoItem *item)
        : m_item(item)
    {
    }

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) override
    {
        QOpenGLFramebufferObjectFormat format;
        format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
        return new QOpenGLFramebufferObject(size, format);
    }

    void synchronize(QQuickFramebufferObject *item) override
    {
        m_item = static_cast<MpvVideoItem *>(item);
    }

    void render() override
    {
        if (!m_item) {
            return;
        }

        if (!m_initialized) {
            initializeOpenGLFunctions();
            m_item->initializeRenderContext(QOpenGLContext::currentContext());
            m_initialized = true;
        }

        glClearColor(0.02f, 0.03f, 0.04f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (!m_item->m_renderContext) {
            return;
        }

        mpv_opengl_fbo fbo {
            static_cast<int>(framebufferObject()->handle()),
            framebufferObject()->width(),
            framebufferObject()->height(),
            0
        };
        int flipY = 0;
        mpv_render_param params[] = {
            {MPV_RENDER_PARAM_OPENGL_FBO, &fbo},
            {MPV_RENDER_PARAM_FLIP_Y, &flipY},
            {MPV_RENDER_PARAM_INVALID, nullptr}
        };
        mpv_render_context_render(m_item->m_renderContext, params);
    }

private:
    MpvVideoItem *m_item = nullptr;
    bool m_initialized = false;
};

MpvVideoItem::MpvVideoItem(QQuickItem *parent)
    : QQuickFramebufferObject(parent)
{
    initializeMpv();
}

MpvVideoItem::~MpvVideoItem()
{
    destroyRenderContext();

    if (m_mpv) {
        mpv_terminate_destroy(m_mpv);
        m_mpv = nullptr;
    }
}

QQuickFramebufferObject::Renderer *MpvVideoItem::createRenderer() const
{
    return new MpvRenderer(const_cast<MpvVideoItem *>(this));
}

QString MpvVideoItem::mediaPath() const
{
    return m_mediaPath;
}

void MpvVideoItem::setMediaPath(const QString &mediaPath)
{
    if (m_mediaPath == mediaPath) {
        return;
    }

    m_mediaPath = mediaPath;
    emit mediaPathChanged();
    maybeLoadFile();
}

qint64 MpvVideoItem::startPositionMs() const
{
    return m_startPositionMs;
}

void MpvVideoItem::setStartPositionMs(qint64 startPositionMs)
{
    if (m_startPositionMs == startPositionMs) {
        return;
    }

    m_startPositionMs = startPositionMs;
    emit startPositionMsChanged();
}

int MpvVideoItem::requestId() const
{
    return m_requestId;
}

void MpvVideoItem::setRequestId(int requestId)
{
    if (m_requestId == requestId) {
        return;
    }

    m_requestId = requestId;
    emit requestIdChanged();
    maybeLoadFile();
}

bool MpvVideoItem::paused() const
{
    return m_paused;
}

bool MpvVideoItem::loaded() const
{
    return m_loaded;
}

qint64 MpvVideoItem::positionMs() const
{
    return m_positionMs;
}

qint64 MpvVideoItem::durationMs() const
{
    return m_durationMs;
}

QString MpvVideoItem::subtitleTrackLabel() const
{
    return m_subtitleTrackLabel;
}

QString MpvVideoItem::audioTrackLabel() const
{
    return m_audioTrackLabel;
}

int MpvVideoItem::volume() const
{
    return m_volume;
}

void MpvVideoItem::togglePause()
{
    if (!m_mpv) {
        return;
    }

    command({QByteArrayLiteral("cycle"), QByteArrayLiteral("pause")});
}

void MpvVideoItem::seekForward()
{
    command({QByteArrayLiteral("seek"), QByteArray::number(kSeekStepSeconds), QByteArrayLiteral("relative")});
}

void MpvVideoItem::seekBackward()
{
    command({QByteArrayLiteral("seek"), QByteArray::number(-kSeekStepSeconds), QByteArrayLiteral("relative")});
}

void MpvVideoItem::cycleSubtitles()
{
    command({QByteArrayLiteral("cycle"), QByteArrayLiteral("sid")});
}

void MpvVideoItem::cycleAudioTracks()
{
    command({QByteArrayLiteral("cycle"), QByteArrayLiteral("aid")});
}

void MpvVideoItem::volumeUp()
{
    command({QByteArrayLiteral("add"), QByteArrayLiteral("volume"), QByteArray::number(kVolumeStep)});
}

void MpvVideoItem::volumeDown()
{
    command({QByteArrayLiteral("add"), QByteArrayLiteral("volume"), QByteArray::number(-kVolumeStep)});
}

void MpvVideoItem::stop()
{
    if (!m_mpv) {
        return;
    }

    command({QByteArrayLiteral("stop")});
    setLoaded(false);
    setPaused(false);
    setPositionMs(0);
    setDurationMs(0);
}

void MpvVideoItem::processMpvEvents()
{
    if (!m_mpv) {
        return;
    }

    while (true) {
        mpv_event *event = mpv_wait_event(m_mpv, 0);
        if (!event || event->event_id == MPV_EVENT_NONE) {
            break;
        }

        switch (event->event_id) {
        case MPV_EVENT_FILE_LOADED:
            setLoaded(true);
            refreshTrackLabels();
            if (m_pendingSeekPositionMs > 0) {
                const double seconds = static_cast<double>(m_pendingSeekPositionMs) / 1000.0;
                QByteArray secondsText = QByteArray::number(seconds, 'f', 3);
                command({QByteArrayLiteral("seek"), secondsText, QByteArrayLiteral("absolute")});
                m_pendingSeekPositionMs = 0;
            }
            break;
        case MPV_EVENT_END_FILE: {
            const auto *endFile = static_cast<mpv_event_end_file *>(event->data);
            const bool wasNearEnd = m_durationMs > 0 && m_positionMs >= qMax<qint64>(0, m_durationMs - 60000);
            setLoaded(false);
            setPositionMs(0);
            if ((endFile && endFile->reason == MPV_END_FILE_REASON_EOF) || wasNearEnd) {
                emit playbackFinished();
            } else {
                emit playbackStopped();
            }
            break;
        }
        case MPV_EVENT_PROPERTY_CHANGE: {
            const auto *property = static_cast<mpv_event_property *>(event->data);
            if (!property) {
                break;
            }

            if (qstrcmp(property->name, "sid") == 0 || qstrcmp(property->name, "aid") == 0
                    || qstrcmp(property->name, "track-list") == 0) {
                refreshTrackLabels();
            } else if (!property->data) {
                break;
            } else if (qstrcmp(property->name, "pause") == 0 && property->format == MPV_FORMAT_FLAG) {
                setPaused(*static_cast<int *>(property->data) != 0);
            } else if (qstrcmp(property->name, "time-pos") == 0 && property->format == MPV_FORMAT_DOUBLE) {
                setPositionMs(static_cast<qint64>(*static_cast<double *>(property->data) * 1000.0));
            } else if (qstrcmp(property->name, "duration") == 0 && property->format == MPV_FORMAT_DOUBLE) {
                setDurationMs(static_cast<qint64>(*static_cast<double *>(property->data) * 1000.0));
            } else if (qstrcmp(property->name, "volume") == 0 && property->format == MPV_FORMAT_DOUBLE) {
                setVolume(static_cast<int>(*static_cast<double *>(property->data)));
            }
            break;
        }
        default:
            break;
        }
    }
}

void MpvVideoItem::maybeLoadFile()
{
    if (!m_mpv || !m_renderContext || m_mediaPath.isEmpty() || m_requestId <= 0 || m_requestId == m_loadedRequestId) {
        return;
    }

    m_loadedRequestId = m_requestId;
    m_pendingSeekPositionMs = m_startPositionMs;
    setLoaded(false);
    setPositionMs(0);
    setDurationMs(0);
    setSubtitleTrackLabel(QStringLiteral("Subtitles Off"));
    setAudioTrackLabel({});

    command({QByteArrayLiteral("loadfile"), m_mediaPath.toUtf8(), QByteArrayLiteral("replace")});
}

void MpvVideoItem::initializeMpv()
{
    m_mpv = mpv_create();
    if (!m_mpv) {
        return;
    }

    mpv_set_option_string(m_mpv, "terminal", "no");
    mpv_set_option_string(m_mpv, "msg-level", "all=warn");
    mpv_set_option_string(m_mpv, "keep-open", "no");
    mpv_set_option_string(m_mpv, "osc", "no");
    mpv_set_option_string(m_mpv, "input-default-bindings", "no");
    mpv_set_option_string(m_mpv, "hwdec", "auto-safe");
    mpv_set_option_string(m_mpv, "vo", "libmpv");
    mpv_set_option_string(m_mpv, "force-window", "no");

    mpv_set_wakeup_callback(m_mpv, &MpvVideoItem::onMpvWakeup, this);

    if (mpv_initialize(m_mpv) < 0) {
        mpv_terminate_destroy(m_mpv);
        m_mpv = nullptr;
        return;
    }

    mpv_observe_property(m_mpv, 0, "pause", MPV_FORMAT_FLAG);
    mpv_observe_property(m_mpv, 0, "time-pos", MPV_FORMAT_DOUBLE);
    mpv_observe_property(m_mpv, 0, "duration", MPV_FORMAT_DOUBLE);
    mpv_observe_property(m_mpv, 0, "sid", MPV_FORMAT_STRING);
    mpv_observe_property(m_mpv, 0, "aid", MPV_FORMAT_STRING);
    mpv_observe_property(m_mpv, 0, "track-list", MPV_FORMAT_NODE);
    mpv_observe_property(m_mpv, 0, "volume", MPV_FORMAT_DOUBLE);
}

void MpvVideoItem::initializeRenderContext(QOpenGLContext *context)
{
    if (!m_mpv || m_renderContext || !context) {
        return;
    }

    static int advancedControl = 1;
    mpv_opengl_init_params glInitParams {
        &MpvVideoItem::getProcAddress,
        context
    };
    mpv_render_param params[] = {
        {MPV_RENDER_PARAM_API_TYPE, const_cast<char *>(MPV_RENDER_API_TYPE_OPENGL)},
        {MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &glInitParams},
        {MPV_RENDER_PARAM_ADVANCED_CONTROL, &advancedControl},
        {MPV_RENDER_PARAM_INVALID, nullptr}
    };

    if (mpv_render_context_create(&m_renderContext, m_mpv, params) < 0) {
        m_renderContext = nullptr;
        return;
    }

    mpv_render_context_set_update_callback(m_renderContext, &MpvVideoItem::onMpvUpdate, this);
    maybeLoadFile();
}

void MpvVideoItem::destroyRenderContext()
{
    if (!m_renderContext) {
        return;
    }

    mpv_render_context_free(m_renderContext);
    m_renderContext = nullptr;
}

void MpvVideoItem::setPaused(bool paused)
{
    if (m_paused == paused) {
        return;
    }

    m_paused = paused;
    emit pausedChanged();
}

void MpvVideoItem::setLoaded(bool loaded)
{
    if (m_loaded == loaded) {
        return;
    }

    m_loaded = loaded;
    emit loadedChanged();
}

void MpvVideoItem::setPositionMs(qint64 positionMs)
{
    if (m_positionMs == positionMs) {
        return;
    }

    m_positionMs = positionMs;
    emit positionChanged();
}

void MpvVideoItem::setDurationMs(qint64 durationMs)
{
    if (m_durationMs == durationMs) {
        return;
    }

    m_durationMs = durationMs;
    emit durationChanged();
}

void MpvVideoItem::setVolume(int volume)
{
    if (m_volume == volume) {
        return;
    }

    m_volume = volume;
    emit volumeChanged();
}

void MpvVideoItem::refreshTrackLabels()
{
    setSubtitleTrackLabel(selectedTrackLabel("sub", QStringLiteral("Subtitles"), QStringLiteral("Subtitles Off")));
    setAudioTrackLabel(selectedTrackLabel("audio", QStringLiteral("Audio"), QStringLiteral("Audio Default")));
}

void MpvVideoItem::setSubtitleTrackLabel(const QString &subtitleTrackLabel)
{
    if (m_subtitleTrackLabel == subtitleTrackLabel) {
        return;
    }

    m_subtitleTrackLabel = subtitleTrackLabel;
    emit subtitleTrackLabelChanged();
}

void MpvVideoItem::setAudioTrackLabel(const QString &audioTrackLabel)
{
    if (m_audioTrackLabel == audioTrackLabel) {
        return;
    }

    m_audioTrackLabel = audioTrackLabel;
    emit audioTrackLabelChanged();
}

QString MpvVideoItem::selectedTrackLabel(const char *type, const QString &enabledPrefix, const QString &disabledLabel) const
{
    const QByteArray prefix = QByteArrayLiteral("current-tracks/") + type + QByteArrayLiteral("/");
    const QString id = propertyString(prefix + QByteArrayLiteral("id"));
    if (id.isEmpty() || id == QStringLiteral("no") || id == QStringLiteral("auto")) {
        return disabledLabel;
    }

    QStringList details;
    const QString title = propertyString(prefix + QByteArrayLiteral("title"));
    const QString lang = propertyString(prefix + QByteArrayLiteral("lang"));

    if (!title.isEmpty()) {
        details.append(title);
    }
    if (!lang.isEmpty()) {
        details.append(lang.toUpper());
    }
    if (details.isEmpty()) {
        details.append(QStringLiteral("Track %1").arg(id));
    }

    return QStringLiteral("%1: %2").arg(enabledPrefix, details.join(QStringLiteral(" - ")));
}

QString MpvVideoItem::propertyString(const QByteArray &name) const
{
    if (!m_mpv) {
        return {};
    }

    char *value = mpv_get_property_string(m_mpv, name.constData());
    if (!value) {
        return {};
    }

    const QString result = QString::fromUtf8(value);
    mpv_free(value);
    return result;
}

void MpvVideoItem::command(const QList<QByteArray> &arguments)
{
    if (!m_mpv || arguments.isEmpty()) {
        return;
    }

    QList<QByteArray> storage = arguments;
    QVector<const char *> commandArguments;
    commandArguments.reserve(storage.size() + 1);
    for (const QByteArray &argument : storage) {
        commandArguments.push_back(argument.constData());
    }
    commandArguments.push_back(nullptr);

    mpv_command_async(m_mpv, 0, const_cast<const char **>(commandArguments.constData()));
}

void MpvVideoItem::onMpvWakeup(void *context)
{
    auto *item = static_cast<MpvVideoItem *>(context);
    QMetaObject::invokeMethod(item, &MpvVideoItem::processMpvEvents, Qt::QueuedConnection);
}

void MpvVideoItem::onMpvUpdate(void *context)
{
    auto *item = static_cast<MpvVideoItem *>(context);
    QMetaObject::invokeMethod(item, [item]() { item->update(); }, Qt::QueuedConnection);
}

void *MpvVideoItem::getProcAddress(void *context, const char *name)
{
    auto *glContext = static_cast<QOpenGLContext *>(context);
    if (!glContext) {
        return nullptr;
    }

    return reinterpret_cast<void *>(glContext->getProcAddress(QByteArray(name)));
}
