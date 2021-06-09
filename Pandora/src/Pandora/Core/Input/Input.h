#pragma once

#include "Pandora/Core/Input/Key.h"
#include "Pandora/Core/Math/VectorInt.h"

namespace pd {

/**
 * \param key The key.
 * \return Whether or not the key is down.
 */
bool IsKeyDown(Key key);

/**
 * \param button The button.
 * \return Whether or not the button is down.
 */
bool IsMouseDown(MouseButton button);

/**
 * \return The current mouse position relative to the window.
 */
Vec2i GetScreenMousePosition();

}
