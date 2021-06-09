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

    /**
     * \brief Initializes the camera with an orthographic projection.
     *
     * \param size The camera size.
     * \param flipVertical Whether or not the projection should be flipped vertically.
     */
    void Init(Vec2i size, bool flipVertical = false);

    /**
     * \brief Initializes the camera with a perspective projection.
     *
     * \param size The camera size.
     * \param fov The camera field-of-view in radians.
     */
    void Init(Vec2i size, f32 fov);

    /**
     * \return The projection type of the camera.
     */
    Projection GetProjectionType() const;

    /**
     * \brief Updates the projection.
     */
    void UpdateProjection();

    /**
     * \return The scaled, translated projection matrix.
     */
    Mat4 GetMatrix() const;

    /**
     * \return The unscaled, untranslated projection matrix.
     */
    Mat4 GetProjectionMatrix() const;

    /**
     * \return The scaled, translated matrix identity matrix.
     */
    Mat4 GetViewMatrix() const;

    /**
     * \brief Converts a screen point to a world point.
     *
     * \param screen The screen point.
     * \return The world point.
     */
    Vec3 ScreenToWorld(Vec2 screen);

    /**
     * \param position The position.
     * \param size The size.
     * \return Checks if the bounding box is visible on the camera.
     */
    bool IsVisible(Vec2 position, Vec2 size);

    /**
     * \return The camera size.
     */
    Vec2i GetSize() const;

    /**
     * \brief The up axis.
     */
    Vec3 up = Vec3(0.0f, 1.0f, 0.0f);

    /**
     * \brief The forward axis.
     */
    Vec3 forward = Vec3(0.0f, 0.0f, -1.0f);

    Vec3 position;
    f32 scale = 1.0f;

    f32 nearZ = 0.1f;
    f32 farZ = 1000.0f;

private:
    Projection projection = Projection::Orthographic;
    Mat4 projMatrix;
    Mat4 view;
    Mat4 translated;

    Vec2i size;
};

}
