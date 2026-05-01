#include "ControllerInput.h"

#include <QDebug>

namespace {

constexpr int kAxisDeadzone = 16000;
constexpr qint64 kInitialRepeatDelayMs = 500;
constexpr qint64 kRepeatIntervalMs = 180;

int directionIndex(ControllerInput::Direction direction)
{
    return static_cast<int>(direction);
}

QString actionForDirection(ControllerInput::Direction direction)
{
    switch (direction) {
    case ControllerInput::Direction::Up:
        return QStringLiteral("up");
    case ControllerInput::Direction::Down:
        return QStringLiteral("down");
    case ControllerInput::Direction::Left:
        return QStringLiteral("left");
    case ControllerInput::Direction::Right:
        return QStringLiteral("right");
    case ControllerInput::Direction::Count:
        break;
    }

    return {};
}

}

ControllerInput::ControllerInput(QObject *parent)
    : QObject(parent)
{
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK) != 0) {
        qWarning() << "SDL controller initialization failed:" << SDL_GetError();
        return;
    }

    m_sdlReady = true;
    m_repeatClock.start();
    openInitialControllers();

    connect(&m_pollTimer, &QTimer::timeout, this, &ControllerInput::pollEvents);
    m_pollTimer.start(16);
}

ControllerInput::~ControllerInput()
{
    if (m_controller != nullptr) {
        SDL_GameControllerClose(m_controller);
    }

    if (m_sdlReady) {
        SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER | SDL_INIT_JOYSTICK);
    }
}

bool ControllerInput::available() const
{
    return m_controller != nullptr;
}

QString ControllerInput::controllerName() const
{
    return m_controllerName;
}

void ControllerInput::openInitialControllers()
{
    const int joystickCount = SDL_NumJoysticks();
    for (int index = 0; index < joystickCount; ++index) {
        if (SDL_IsGameController(index)) {
            openController(index);
            return;
        }
    }
}

void ControllerInput::openController(int deviceIndex)
{
    if (m_controller != nullptr || !SDL_IsGameController(deviceIndex)) {
        return;
    }

    SDL_GameController *controller = SDL_GameControllerOpen(deviceIndex);
    if (controller == nullptr) {
        qWarning() << "Failed to open SDL controller:" << SDL_GetError();
        return;
    }

    SDL_Joystick *joystick = SDL_GameControllerGetJoystick(controller);
    m_controller = controller;
    m_controllerInstanceId = joystick != nullptr ? SDL_JoystickInstanceID(joystick) : -1;
    m_controllerName = QString::fromUtf8(SDL_GameControllerName(controller));
    updateControllerState();
}

void ControllerInput::closeController(SDL_JoystickID instanceId)
{
    if (m_controller == nullptr || m_controllerInstanceId != instanceId) {
        return;
    }

    SDL_GameControllerClose(m_controller);
    m_controller = nullptr;
    m_controllerInstanceId = -1;
    m_controllerName.clear();
    m_directionPressed.fill(false);
    m_nextRepeatMs.fill(0);
    updateControllerState();
    openInitialControllers();
}

void ControllerInput::pollEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
        case SDL_CONTROLLERDEVICEADDED:
            openController(event.cdevice.which);
            break;
        case SDL_CONTROLLERDEVICEREMOVED:
            closeController(event.cdevice.which);
            break;
        case SDL_CONTROLLERBUTTONDOWN:
            handleControllerButton(event.cbutton);
            break;
        case SDL_CONTROLLERBUTTONUP:
            if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP) {
                setDirectionPressed(Direction::Up, false);
            } else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN) {
                setDirectionPressed(Direction::Down, false);
            } else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT) {
                setDirectionPressed(Direction::Left, false);
            } else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) {
                setDirectionPressed(Direction::Right, false);
            }
            break;
        case SDL_CONTROLLERAXISMOTION:
            handleControllerAxis(event.caxis);
            break;
        default:
            break;
        }
    }

    repeatHeldDirections();
}

void ControllerInput::handleControllerButton(const SDL_ControllerButtonEvent &event)
{
    if (event.which != m_controllerInstanceId) {
        return;
    }

    switch (event.button) {
    case SDL_CONTROLLER_BUTTON_A:
        emitAction(QStringLiteral("accept"));
        break;
    case SDL_CONTROLLER_BUTTON_B:
        emitAction(QStringLiteral("cancel"));
        break;
    case SDL_CONTROLLER_BUTTON_X:
        emitAction(QStringLiteral("x"));
        break;
    case SDL_CONTROLLER_BUTTON_Y:
        emitAction(QStringLiteral("y"));
        break;
    case SDL_CONTROLLER_BUTTON_BACK:
        emitAction(QStringLiteral("quit"));
        break;
    case SDL_CONTROLLER_BUTTON_START:
        emitAction(QStringLiteral("playPause"));
        break;
    case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
        emitAction(QStringLiteral("leftShoulder"));
        break;
    case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
        emitAction(QStringLiteral("rightShoulder"));
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_UP:
        setDirectionPressed(Direction::Up, true);
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
        setDirectionPressed(Direction::Down, true);
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
        setDirectionPressed(Direction::Left, true);
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
        setDirectionPressed(Direction::Right, true);
        break;
    default:
        break;
    }
}

void ControllerInput::handleControllerAxis(const SDL_ControllerAxisEvent &event)
{
    if (event.which != m_controllerInstanceId) {
        return;
    }

    if (event.axis == SDL_CONTROLLER_AXIS_LEFTX) {
        setDirectionPressed(Direction::Left, event.value < -kAxisDeadzone);
        setDirectionPressed(Direction::Right, event.value > kAxisDeadzone);
    } else if (event.axis == SDL_CONTROLLER_AXIS_LEFTY) {
        setDirectionPressed(Direction::Up, event.value < -kAxisDeadzone);
        setDirectionPressed(Direction::Down, event.value > kAxisDeadzone);
    }
}

void ControllerInput::setDirectionPressed(Direction direction, bool pressed)
{
    const int index = directionIndex(direction);
    if (m_directionPressed[index] == pressed) {
        return;
    }

    m_directionPressed[index] = pressed;
    if (pressed) {
        m_nextRepeatMs[index] = m_repeatClock.elapsed() + kInitialRepeatDelayMs;
        emitAction(actionForDirection(direction));
    } else {
        m_nextRepeatMs[index] = 0;
    }
}

void ControllerInput::repeatHeldDirections()
{
    const qint64 now = m_repeatClock.elapsed();
    for (int index = 0; index < static_cast<int>(Direction::Count); ++index) {
        if (!m_directionPressed[index] || now < m_nextRepeatMs[index]) {
            continue;
        }

        const auto direction = static_cast<Direction>(index);
        emitAction(actionForDirection(direction));
        m_nextRepeatMs[index] = now + kRepeatIntervalMs;
    }
}

void ControllerInput::emitAction(QString action)
{
    if (!action.isEmpty()) {
        emit actionPressed(action);
    }
}

void ControllerInput::updateControllerState()
{
    emit availableChanged();
    emit controllerNameChanged();
}
