#pragma once

#include "Pandora/Core/Input/Key.h"
#include "Pandora/Core/Math/Vector.h"
#include "Pandora/Core/Event.h"

namespace pd {

class InputManager {
public:
    /// <summary>
    /// Returns whether the or not key is pressed.
    /// A pressed key is down this frame and up last frame.
    /// </summary>
    /// <param name="key">The key.</param>
    /// <returns>Whether the or not key is pressed.</returns>
    bool IsKeyPressed(Key key) const;

    /// <summary>
    /// Returns whether the or not key is down this frame.
    /// </summary>
    /// <param name="key">The key.</param>
    /// <returns>Whether the or not key is down.</returns>
    bool IsKeyDown(Key key) const;

    /// <summary>
    /// Returns whether the or not key is down previous frame.
    /// </summary>
    /// <param name="key">The key.</param>
    /// <returns>Whether the or not key is down.</returns>
    bool WasKeyDown(Key key) const;

    bool IsKeyRepeated(Key key) const;

    /// <summary>
    /// Returns whether the or not button is pressed.
    /// A pressed button is down this frame and up last frame.
    /// </summary>
    /// <param name="button">The button.</param>
    /// <returns>Whether the or not button is pressed.</returns>
    bool IsMousePressed(MouseButton button) const;

    /// <summary>
    /// Returns whether the or not button is down this frame.
    /// </summary>
    /// <param name="button">The button.</param>
    /// <returns>Whether the or not button is down.</returns>
    bool IsMouseDown(MouseButton button) const;

    /// <summary>
    /// Returns whether the or not button is down previous frame.
    /// </summary>
    /// <param name="button">The button.</param>
    /// <returns>Whether the or not button is down.</returns>
    bool WasMouseDown(MouseButton button) const;
    
    /// <summary>
    /// Returns the mouse position relative to the window. (0, 0) is bottom left.
    /// </summary>
    /// <returns>The mouse position relative to the window.</returns>
    Vec2 GetMousePos() const;
    
    /// <summary>
    /// Returns the scroll wheel state.
    /// </summary>
    /// <returns>The scroll wheel state.</returns>
    Vec2 GetMouseWheel() const;

    /// <summary>
    /// Updates the input state so that all the current state becomes last frame's state.
    /// </summary>
    void Update();
    
    /// <summary>
    /// Uses input events.
    /// </summary>
    /// <param name="event">The event.</param>
    void OnEvent(Event* event);

private:
    struct InputState {
        bool isDown = false;
        bool wasDown = false;
        bool isRepeated = false;
    };

    InputState keys[(int)Key::Count];
    InputState buttons[(int)MouseButton::Count];

    Vec2 mousePos;
    Vec2 mouseWheel;
};

}
