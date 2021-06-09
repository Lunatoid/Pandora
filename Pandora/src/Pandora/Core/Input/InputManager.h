#pragma once

#include "Pandora/Core/Input/Key.h"
#include "Pandora/Core/Math/Vector.h"
#include "Pandora/Core/Event.h"

namespace pd {

class InputManager {
public:

    /**
     * \param key The key.
     * \return Whether or not the key has been pressed.
     */
    bool IsKeyPressed(Key key) const;

    /**
     * \param key The key.
     * \return Whether or not the key is currently down.
     */
    bool IsKeyDown(Key key) const;

    /**
     * \param key The key.
     * \return Whether or not the key was down last frame.
     */
    bool WasKeyDown(Key key) const;

    /**
     * \param key The key.
     * \return Whether or not the key has been repeated.
     */
    bool IsKeyRepeated(Key key) const;

    /**
     * \param button The button.
     * \return Whether or not the button has been pressed.
     */
    bool IsMousePressed(MouseButton button) const;
 
    /**
     * \param button The button.
     * \return Whether or not the button is currently down.
     */
    bool IsMouseDown(MouseButton button) const;

    /**
     * \param button The button.
     * \return Whether or not the button was down last frame.
     */
    bool WasMouseDown(MouseButton button) const;

    /**
     * \return The mouse position relative to the window. (0, 0) is the bottom left.
     */
    Vec2 GetMousePos() const;

    /**
     * \return The scroll wheel state.
     */
    Vec2 GetMouseWheel() const;

    /**
     * \brief Updates the input state.
     */
    void Update();

    /**
     * \brief Processes the input events.
     * 
     * \param event The event.
     */
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
