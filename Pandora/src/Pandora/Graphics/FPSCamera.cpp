#include "FPSCamera.h"

#include "Pandora/Core/Window/WindowEvent.h"
#include "Pandora/Core/Input/Input.h"
#include "Pandora/Core/Math/Math.h"

namespace pd {

void FPSCamera::HandleInput(Event* event) {
    if (event->category == EventCategory::Window) {
        WindowEvent* we = (WindowEvent*)event;

        if (we->type == WindowEventType::MouseMove) {
            f32 finalSensitivity = sensitivity;
            yaw += we->mouseMove.delta.x * finalSensitivity;
            pitch += we->mouseMove.delta.y * finalSensitivity;

            pitch = Clamp(pitch, -89.0f, 89.0f);
        }
    }
}


void FPSCamera::Update(f32 dt, InputManager& input, Window& window) {
    if (isCapturingMouse) {
        // Movement
        f32 finalSpeed = speed;

        if (IsKeyDown(keySpeedup)) {
            finalSpeed *= speedupMod;
        }

        finalSpeed *= dt;

        f32 oldY = position.y;

        if (IsKeyDown(keyForward)) {
            position += finalSpeed * forward;
        }

        if (IsKeyDown(keyLeft)) {
            position -= finalSpeed * Normalize(Cross(forward, up));
        }

        if (IsKeyDown(keyBackward)) {
            position -= finalSpeed * forward;
        }

        if (IsKeyDown(keyRight)) {
            position += finalSpeed * Normalize(Cross(forward, up));
        }

        if (lockYAxis) {
            position.y = oldY;
        }

        // Looking
        Vec3 front;
        front.x = Cos(ToRadians(yaw)) * Cos(ToRadians(pitch));
        front.y = Sin(ToRadians(pitch));
        front.z = Sin(ToRadians(yaw)) * Cos(ToRadians(pitch));

        forward = Normalize(front);
    }

    UpdateProjection();

    // Cursor lock
    if (freeCursorWithEsc) {
        if (input.IsKeyPressed(Key::Escape)) {
            isCapturingMouse = !isCapturingMouse;

            window.CaptureMouse(isCapturingMouse);
            window.SetRelativeMouseMode(isCapturingMouse);
            window.SetCursorVisible(!isCapturingMouse);
        }
    }
}

}
