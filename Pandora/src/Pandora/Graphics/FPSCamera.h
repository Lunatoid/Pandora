#pragma once

#include "Pandora/Core/Event.h"

#include "Pandora/Core/Input/Key.h"
#include "Pandora/Core/Input/InputManager.h"

#include "Pandora/Core/Window/Window.h"

#include "Pandora/Graphics/Camera.h"

namespace pd {

class FPSCamera : public Camera {
public:    
    /// <summary>
    /// Handles the mouse motion events.
    /// The window must be in relative mouse mode.
    /// </summary>
    /// <param name="event">The event.</param>
    void HandleInput(Event* event);
    
    /// <summary>
    /// Does movement code and updates the projection.
    /// </summary>
    /// <param name="dt">The delta time.</param>
    /// <param name="input">The input manager.</param>
    /// <param name="window">The window.</param>
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
    f32 pitch = 0.0f;
    f32 yaw = -90.0f;

    bool isCapturingMouse = true;
};

}
