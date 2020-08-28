#pragma once

#include "Pandora/Core/Input/Key.h"

#include "Pandora/Core/Math/VectorInt.h"

namespace pd {

/// <summary>
/// Checks if a key is pressed down.
/// </summary>
/// <param name="key">The key.</param>
/// <returns>Whether or not the key is down.</returns>
bool IsKeyDown(Key key);

/// <summary>
/// Checks if a mouse button is pressed down.
/// </summary>
/// <param name="button">The button.</param>
/// <returns>Whether or not the button is down.</returns>
bool IsMouseDown(MouseButton button);

Vec2i GetScreenMousePosition();

void SetScreenMousePosition(Vec2i position);

}
