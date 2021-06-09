#include "InputManager.h"

#include "Pandora/Core/Window/WindowEvent.h"

namespace pd {

bool InputManager::IsKeyPressed(Key key) const {
    return keys[(int)key].isDown && !keys[(int)key].wasDown;
}

bool InputManager::IsKeyDown(Key key) const {
    return keys[(int)key].isDown;
}

bool InputManager::WasKeyDown(Key key) const {
    return keys[(int)key].wasDown;
}

bool InputManager::IsKeyRepeated(Key key) const {
    return keys[(int)key].isRepeated;
}

bool InputManager::IsMousePressed(MouseButton button) const {
    return buttons[(int)button].isDown && !buttons[(int)button].wasDown;
}

bool InputManager::IsMouseDown(MouseButton button) const {
    return buttons[(int)button].isDown;
}

bool InputManager::WasMouseDown(MouseButton button) const {
    return buttons[(int)button].wasDown;
}

Vec2 InputManager::GetMousePos() const {
    return mousePos;
}

Vec2 InputManager::GetMouseWheel() const {
    return mouseWheel;
}

void InputManager::Update() {
    for (int i = 0; i < (int)Key::Count; i++) {
        keys[i].wasDown = keys[i].isDown;
        keys[i].isRepeated = false;
    }

    for (int i = 0; i < (int)MouseButton::Count; i++) {
        buttons[i].wasDown = buttons[i].isDown;
    }

    mouseWheel = Vec2(0.0f);
}

void InputManager::OnEvent(Event* event) {
    if (event->category == EventCategory::Window) {
        WindowEvent* we = (WindowEvent*)event;

        switch (we->type) {
            case WindowEventType::KeyUp:
            case WindowEventType::KeyDown: {
                keys[(int)we->keyPress.key].isDown = we->keyPress.isDown;
                break;
            }

            case WindowEventType::KeyRepeat: {
                keys[(int)we->keyRepeat.key].isRepeated = true;
                break;
            }

            case WindowEventType::MouseButtonUp:
            case WindowEventType::MouseButtonDown: {
                buttons[(int)we->mousePress.button].isDown = we->mousePress.isDown;
                break;
            }

            case WindowEventType::MouseMove: {
                mousePos = we->mouseMove.position;
                break;
            }

            case WindowEventType::MouseWheel: {
                mouseWheel = Vec2(we->mouseWheel.x, we->mouseWheel.y);
                break;
            }
        }
    }
}

}
