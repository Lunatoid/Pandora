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

    void UpdateProjection(Mat4 mvp);

    // @TODO: frustrum culling
    
    /// <summary>
    /// Queues the specified sprite for drawing.
    /// </summary>
    /// <param name="sprite">The sprite.</param>
    void Draw(Sprite& sprite);
    
    /// <summary>
    /// Renders all queued sprites.
    /// </summary>
    void Render();
    
    /// <summary>
    /// Returns the max depth the sprites we're drawn to.
    /// </summary>
    /// <returns>The max depth the sprites we're drawn to.</returns>
    f32 GetMaxDepth() const;

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
    Mat4 mvp = Mat4(1.0f);
    f32 maxDepth;

    Ref<Renderer> renderer;
    Ref<ConstantBuffer> mvpBuffer;

    // We might want a seperate array of pointers to sort because it'll be quicker.
    Array<Sprite> sprites;
};

}
