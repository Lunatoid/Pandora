#pragma once

#include "Pandora/Core/Event.h"

#include "Pandora/Core/Math/VectorInt.h"

#include "Pandora/Core/Input/Key.h"

namespace pd {

enum class WindowEventType : u8 {
    None,
    Close,
    Resize,
    GainedFocus,
    LostFocus,
    KeyDown,
    KeyUp,
    KeyRepeat,
    TextDown,
    MouseButtonDown,
    MouseButtonUp,
    MouseWheel,
    MouseMove
};

struct WindowEvent final : public Event {
    WindowEvent() {
        category = EventCategory::Window;

        // To make the compiler shut up about uninitialized member variables
        resize = {};
        keyPress = {};
        keyRepeat = {};
        textDown = {};
        mousePress = {};
        mouseWheel = {};
        mouseMove = {};
    }

    WindowEventType type = WindowEventType::None;

    union {
        struct {
            Vec2i newSize;
        } resize;

        struct {
            Key key;
            bool isDown;
        } keyPress;

        struct {
            Key key;
            u16 repeatCount;
        } keyRepeat;

        struct {
            codepoint point;
        } textDown;

        struct {
            MouseButton button;
            Vec2i position;
            bool isDown;
        } mousePress;

        struct {
            f32 x;
            f32 y;
        } mouseWheel;

        struct {
            Vec2i position;
            Vec2i delta;
        } mouseMove;

    };
};

}
