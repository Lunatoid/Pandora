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

    /**
     * \brief Updates the matrix and sets the culling camera.
     * 
     * \param cam The camera.
     */
    void UpdateProjection(Camera cam);

    /**
     * \brief Queues the specified sprite for drawing.
     * 
     * \param sprite The sprite.
     */
    void Draw(const Sprite& sprite);

    /**
     * \brief Renders all the queued sprites.
     */
    void Render();

    /// <summary>
    /// Renders all queued sprites.
    /// </summary>
    /// <param name="startSquishDepth">If <c>squishZ</c> is on, use <c>startSquishDepth</c> as the starting depth.</param>

    /**
     * \brief Renders all the queued sprites.
     * If `squishZ` is true, it will use `startSquishDepth` as the starting depth.
     * 
     * \param startSquishDepth The starting depth is `squishZ` is on.
     */
    void Render(f32 startSquishDepth);

    /**
     * \return The maximum depth that has been drawn so far.
     */
    f32 GetMaxDepth() const;

    /**
     * \brief Resets the maximum dpeth that we've drawn so far.
     * Handy if you're using the max depth + n to draw over everything and you don't
     * want your depth to snowball out of control.
     */
    void ResetMaxDepth();

    /**
     * \brief When `true` the sprite renderer will change the Z value for all the sprites to
     * be squished together. This avoids Z-fighting while still utilizing the depth buffer.
     */
    bool squishZ = true;

    /**
     * \brief The distance between each sprite if `squishZ` is enabled.
     */
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
