#pragma once

#include <QObject>
#include <QElapsedTimer>
#include <QTimer>

#include <SDL.h>

#include <array>

class ControllerInput final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool available READ available NOTIFY availableChanged)
    Q_PROPERTY(QString controllerName READ controllerName NOTIFY controllerNameChanged)

public:
    enum class Direction {
        Up = 0,
        Down,
        Left,
        Right,
        Count
    };

    explicit ControllerInput(QObject *parent = nullptr);
    ~ControllerInput() override;

    bool available() const;
    QString controllerName() const;

signals:
    void availableChanged();
    void controllerNameChanged();
    void actionPressed(const QString &action);

private:
    void openInitialControllers();
    void openController(int deviceIndex);
    void closeController(SDL_JoystickID instanceId);
    void pollEvents();
    void handleControllerButton(const SDL_ControllerButtonEvent &event);
    void handleControllerAxis(const SDL_ControllerAxisEvent &event);
    void setDirectionPressed(Direction direction, bool pressed);
    void repeatHeldDirections();
    void emitAction(QString action);
    void updateControllerState();

    QTimer m_pollTimer;
    QElapsedTimer m_repeatClock;
    std::array<bool, static_cast<int>(Direction::Count)> m_directionPressed = {};
    std::array<qint64, static_cast<int>(Direction::Count)> m_nextRepeatMs = {};
    SDL_GameController *m_controller = nullptr;
    SDL_JoystickID m_controllerInstanceId = -1;
    QString m_controllerName;
    bool m_sdlReady = false;
};
