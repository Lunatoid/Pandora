#include "Camera.h"

#include "Pandora/Core/Assert.h"
#include "Pandora/Core/Math/Math.h"

namespace pd {

void Camera::Init(Vec2i size, bool flipVertical) {
    this->size = size;

    f32 left = -size.x / 2.0f;
    f32 right = -left;
    f32 bottom = (flipVertical) ? size.y / 2.0f : -size.y / 2.0f;
    f32 top = -bottom;

    projMatrix = Orthographic(left, right, bottom, top, nearZ, farZ);
    projection = Projection::Orthographic;

    position.z = farZ;
}

void Camera::Init(Vec2i size, f32 fov) {
    this->size = size;

    projMatrix = Perspective(ToRadians(fov), (f32)size.x / (f32)size.y, nearZ, farZ);
    projection = Projection::Perspective;
}

Projection Camera::GetProjectionType() const {
    return projection;
}

void Camera::UpdateProjection() {
    view = Mat4(1.0f);

    if (scale != 1.0f) {
        view *= Scale(scale);
    }

    view *= LookAt(position, position + forward, up);

    translated = projMatrix * view;
}

Vec3 Camera::ScreenToWorld(Vec2 screen) {
    switch (projection) {
        case Projection::Orthographic: {
            screen -= (Vec2)size / 2.0f;
            screen /= Vec2(scale);
            screen += Vec2(position.x, position.y);
            return screen;
        }

        default:
            PD_ASSERT_D(false, "Unhandled case: ScreenToWorld [ {} ]", projection);
    }

    return Vec2(0.0f);
}

bool Camera::IsVisible(Vec2 position, Vec2 size) {
    switch (projection) {
        case Projection::Orthographic: {
            Vec2 scaledScreenDimension = (Vec2)this->size / scale;

            float xDistance = (size.x / 2.0f) + (scaledScreenDimension.x / 2.0f);
            float yDistance = (size.y / 2.0f) + (scaledScreenDimension.y / 2.0f);

            Vec2 centerPos = position + (size / 2.f);
            Vec2 centerCameraPos = this->position.xy;
            Vec2 distVec = centerPos - centerCameraPos;

            float xDepth = xDistance - Abs(distVec.x);
            float yDepth = yDistance - Abs(distVec.y);

            return xDepth > 0 && yDepth > 0;
        }

        default: {
            // PD_ASSERT_D(false, "Unhandled case: ScreenToWorld [ {} ]", projection);
            return true;
        }
    }

    return false;
}

Mat4 Camera::GetMatrix() const {
    return translated;
}

Mat4 Camera::GetProjectionMatrix() const {
    return projMatrix;
}

Mat4 Camera::GetViewMatrix() const {
    return view;
}

Vec2i Camera::GetSize() const {
    return size;
}

}
