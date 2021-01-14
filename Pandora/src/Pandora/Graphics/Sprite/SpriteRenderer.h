#pragma once

#include "Pandora/Core/Data/Reference.h"

#include "Pandora/Graphics/Renderer.h"
#include "Pandora/Graphics/ConstantBuffer.h"
#include "Pandora/Graphics/Camera.h"
#include "Pandora/Graphics/Sprite/Sprite.h"

namespace pd {

class SpriteRenderer {
public:
    SpriteRenderer();

    /// <summary>
    /// Updates the mvp matrix and uses this camera for culling.
    /// </summary>
    /// <param name="cam">The camera.</param>
    void UpdateProjection(Camera cam);

    /// <summary>
    /// Queues the specified sprite for drawing.
    /// </summary>
    /// <param name="sprite">The sprite.</param>
    void Draw(const Sprite& sprite);

    /// <summary>
    /// Renders all queued sprites.
    /// </summary>
    void Render();

    /// <summary>
    /// Renders all queued sprites.
    /// </summary>
    /// <param name="startSquishDepth">If <c>squishZ</c> is on, use <c>startSquishDepth</c> as the starting depth.</param>
    void Render(f32 startSquishDepth);

    /// <summary>
    /// Returns the max depth the sprites we're drawn to.
    /// </summary>
    /// <returns>The max depth the sprites we're drawn to.</returns>
    f32 GetMaxDepth() const;

    /// <summary>
    /// Resets the recorded max depth.
    /// Handy if you're using the max depth + n to draw over everything and you don't
    /// want your depth to keep increasing out of control
    /// </summary>
    void ResetMaxDepth();

    /// <summary>
    /// If <c>true</c> the sprite renderer will change the Z value to be squished together.
    /// This avoids Z-fighting while still using the depth buffer.
    /// </summary>
    bool squishZ = true;
    
    /// <summary>
    /// The Z-distance between each sprite if <c>squishZ</c> is enabled.
    /// </summary>
    f32 zDistance = 0.1f;

private:
    Camera cam;
    f32 startSquishDepth = 0.0f;
    f32 maxDepth = 0.0f;

    Ref<Renderer> renderer;
    Ref<ConstantBuffer> mvpBuffer;

    // We might want a seperate array of pointers to sort because it'll be quicker.
    Array<Sprite> sprites;
};

}
