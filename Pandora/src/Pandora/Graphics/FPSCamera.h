#pragma once

#include "Pandora/Core/Event.h"
#include "Pandora/Core/Input/Key.h"
#include "Pandora/Core/Input/InputManager.h"
#include "Pandora/Core/Window/Window.h"

#include "Pandora/Graphics/Camera.h"

namespace pd {

class FPSCamera : public Camera {
public:

    /**
     * \brief Processes the mouse motion events.
     * Make sure that the window is in relative mouse mode.
     * 
     * \param event 
     */
    void HandleInput(Event* event);

    /**
     * \brief Updates the projection and handles movement.
     * 
     * \param dt The delta time.
     * \param input The input manager to use.
     * \param window The window to use.
     */
    void Update(f32 dt, InputManager& input, Window& window);

    Key keyForward = Key::W;
    Key keyLeft = Key::A;
    Key keyBackward = Key::S;
    Key keyRight = Key::D;
    Key keySpeedup = Key::LeftShift;
 
    f32 sensitivity = 0.15f;
    f32 speed = 5.0f;
    f32 speedupMod = 3.0f;

    bool lockYAxis = false;

    bool freeCursorWithEsc = true;

private:
    // @TODO: quaternions... maybe...
    f32 pitch = 0.0f;
    f32 yaw = -90.0f;

    bool isCapturingMouse = true;
};

}
