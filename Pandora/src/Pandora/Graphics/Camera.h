#pragma once

#include "Pandora/Core/Math/Matrix.h"
#include "Pandora/Core/Math/VectorInt.h"

namespace pd {

enum class Projection {
    Orthographic,
    Perspective
};

class Camera {
public:    
    /// <summary>
    /// Initializes the camera with an orthographic projection.
    /// </summary>
    /// <param name="size">The size.</param>
    /// <param name="flipVertical">Whether or not to flip the projection vertically.</param>
    void Init(Vec2i size, bool flipVertical = false);
    
    /// <summary>
    /// Initializes the camera with a perspective projection.
    /// </summary>
    /// <param name="size">The size.</param>
    /// <param name="fov">The fov in degrees.</param>
    /// <param name="pixelSize">Size of the pixel.</param>
    void Init(Vec2i size, f32 fov);
    
    /// <summary>
    /// Returns the projection type.
    /// </summary>
    /// <returns>The projection type.</returns>
    Projection GetProjectionType() const;

    /// <summary>
    /// Returns the scaled, translated projection matrix.
    /// </summary>
    /// <returns>The scaled, translated projection matrix.</returns>
    Mat4 GetMatrix();
    
    /// <summary>
    /// Updates the projection.
    /// </summary>
    void UpdateProjection();

    Vec3 up = Vec3(0.0f, 1.0f, 0.0f);
    Vec3 forward = Vec3(0.0f, 0.0f, -1.0f);

    Vec3 position;
    f32 scale = 1.0f;

    f32 nearZ = 0.1f;
    f32 farZ = 1000.0f;
private:
    Projection projection = Projection::Orthographic;
    Mat4 projMatrix;
    Mat4 view;

    Vec2i size;
};

}
