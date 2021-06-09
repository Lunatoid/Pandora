#include "Window.h"

#include "Pandora/Core/Assert.h"

#include "Pandora/Core/Data/String.h"

#include "Pandora/Core/Math/VectorInt.h"

#if !defined(PD_NO_IMGUI)
#include "Pandora/Libs/ImGui/SDL2/imgui_impl_sdl.h"
#endif

#include <SDL2/SDL.h>

namespace pd {

#define SDL_WINDOW ((SDL_Window*)nativeData)

Key KeySymToKey(SDL_Keysym sym);

Window::Window() {}

Window::~Window() {
    Delete();
}

void Window::Delete() {
    SDL_DestroyWindow(SDL_WINDOW);
}

void Window::Open(StringView title, Vec2i size, u32 style, VideoBackend backend) {
    u32 flags = 0;

    switch (backend) {
        case VideoBackend::OpenGL: {
            flags |= SDL_WINDOW_OPENGL;
            break;
        }
    }

    if ((style & WindowResizable) != 0) {
        flags |= SDL_WINDOW_RESIZABLE;
    }

    if ((style & WindowFullscreen) != 0) {
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    nativeData = SDL_CreateWindow(title.CStr(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.x, size.y, flags);

    PD_ASSERT_D(nativeData != nullptr, "SDL window couldn't be created");
    isOpen = true;
    windowSize = size;
    this->backend = backend;
}

void Window::HandleEvents() {
    SDL_Event evnt;
    while (SDL_PollEvent(&evnt)) {
#if !defined(PD_NO_IMGUI)
        ImGui_ImplSDL2_ProcessEvent(&evnt);
#endif

        switch (evnt.type) {
            case SDL_QUIT: {
                isOpen = false;

                WindowEvent windowEvent;
                windowEvent.category = EventCategory::Window;
                windowEvent.type = WindowEventType::Close;

                events.Add(windowEvent);
                break;
            }

            case SDL_WINDOWEVENT: {
                // Translate into our own format
                WindowEvent windowEvent;
                windowEvent.category = EventCategory::Window;

                switch (evnt.window.event) {
                    case SDL_WINDOWEVENT_RESIZED: {
                        windowSize = Vec2i(evnt.window.data1, evnt.window.data2);

                        windowEvent.type = WindowEventType::Resize;
                        windowEvent.resize.newSize = windowSize;
                        break;
                    }

                    case SDL_WINDOWEVENT_FOCUS_GAINED: {
                        windowEvent.type = WindowEventType::GainedFocus;
                        break;
                    }

                    case SDL_WINDOWEVENT_FOCUS_LOST: {
                        windowEvent.type = WindowEventType::LostFocus;
                        break;
                    }
                }

                events.Add(windowEvent);
                break;
            }

            case SDL_KEYDOWN: {
                WindowEvent windowEvent;
                windowEvent.category = EventCategory::Window;

                windowEvent.type = WindowEventType::KeyRepeat;
                windowEvent.keyRepeat.key = KeySymToKey(evnt.key.keysym);
                windowEvent.keyRepeat.repeatCount = evnt.key.repeat; // Check if this actually gives us a repeat value
                events.Add(windowEvent);

                if (evnt.key.repeat == 0) {
                    windowEvent.type = WindowEventType::KeyDown;
                    windowEvent.keyPress.key = KeySymToKey(evnt.key.keysym);
                    windowEvent.keyPress.isDown = true;
                    events.Add(windowEvent);
                }

                break;
            }

            case SDL_KEYUP: {
                WindowEvent windowEvent;
                windowEvent.category = EventCategory::Window;
                windowEvent.type = WindowEventType::KeyUp;
                windowEvent.keyPress.key = KeySymToKey(evnt.key.keysym);
                windowEvent.keyPress.isDown = false;

                events.Add(windowEvent);
                break;
            }

            case SDL_TEXTINPUT: {
                uchar* text = evnt.text.text;
                codepoint p;
                do {
                    text = GetNextCodepoint(text, &p);

                    WindowEvent windowEvent;
                    windowEvent.type = WindowEventType::TextDown;
                    windowEvent.textDown.point = p;

                    events.Add(windowEvent);
                } while (p != 0);

                break;
            }

            case SDL_MOUSEWHEEL: {
                WindowEvent windowEvent;
                windowEvent.category = EventCategory::Window;
                windowEvent.type = WindowEventType::MouseWheel;
                windowEvent.mouseWheel.x = evnt.wheel.x;
                windowEvent.mouseWheel.y = evnt.wheel.y;

                events.Add(windowEvent);
                break;
            }

            case SDL_MOUSEBUTTONDOWN: {
                WindowEvent windowEvent;
                windowEvent.category = EventCategory::Window;
                windowEvent.type = WindowEventType::MouseButtonDown;

                switch (evnt.button.button) {
                    case SDL_BUTTON_LEFT:
                        windowEvent.mousePress.button = MouseButton::Left;
                        break;

                    case SDL_BUTTON_MIDDLE:
                        windowEvent.mousePress.button = MouseButton::Middle;
                        break;

                    case SDL_BUTTON_RIGHT:
                        windowEvent.mousePress.button = MouseButton::Right;
                        break;
                }

                windowEvent.mousePress.isDown = true;
                windowEvent.mousePress.position = Vec2i(evnt.button.x, windowSize.y - evnt.button.y);

                events.Add(windowEvent);
                break;
            }

            case SDL_MOUSEBUTTONUP: {
                WindowEvent windowEvent;
                windowEvent.category = EventCategory::Window;
                windowEvent.type = WindowEventType::MouseButtonUp;

                switch (evnt.button.button) {
                    case SDL_BUTTON_LEFT:
                        windowEvent.mousePress.button = MouseButton::Left;
                        break;

                    case SDL_BUTTON_MIDDLE:
                        windowEvent.mousePress.button = MouseButton::Middle;
                        break;

                    case SDL_BUTTON_RIGHT:
                        windowEvent.mousePress.button = MouseButton::Right;
                        break;
                }

                windowEvent.mousePress.isDown = false;
                windowEvent.mousePress.position = Vec2i(evnt.button.x, windowSize.y - evnt.button.y);

                events.Add(windowEvent);
                break;
            }

            case SDL_MOUSEMOTION: {
                WindowEvent windowEvent;
                windowEvent.category = EventCategory::Window;
                windowEvent.type = WindowEventType::MouseMove;

                windowEvent.mouseMove.delta = Vec2i(evnt.motion.xrel, -evnt.motion.yrel);
                windowEvent.mouseMove.position = Vec2i(evnt.motion.x, windowSize.y - evnt.motion.y);

                events.Add(windowEvent);
                break;
            }
        }
    }
}

bool Window::PollEvent(WindowEvent* out) {
    if (events.Count() > 0) {
        *out = events.First();
        events.Remove(0);
        return true;
    }

    return false;
}

void Window::CaptureMouse(bool shouldCapture) {
    SDL_CaptureMouse((SDL_bool)shouldCapture);
}

void Window::SetMousePosition(Vec2i position) {
    SDL_WarpMouseInWindow(SDL_WINDOW, position.x, position.y);
}

void Window::SetRelativeMouseMode(bool enabled) {
    SDL_SetRelativeMouseMode((SDL_bool)enabled);
}

Vec2i Window::GetSize() {
    return windowSize;
}

bool Window::IsOpen() const {
    return isOpen;
}

bool Window::IsFullscreen() const {
    return (SDL_GetWindowFlags(SDL_WINDOW) & SDL_WINDOW_FULLSCREEN) != 0;
}

void Window::SetWindowVisible(bool isVisible) {
    if (isVisible) {
        SDL_ShowWindow(SDL_WINDOW);
    } else {
        SDL_HideWindow(SDL_WINDOW);
    }
}

void Window::SetCursorVisible(bool isVisible) {
    SDL_ShowCursor((int)isVisible);
}

void Window::SetFullscreen(bool isFullscreen) {
    SDL_SetWindowFullscreen(SDL_WINDOW, (isFullscreen) ? SDL_WINDOW_FULLSCREEN : 0);
}

bool Window::IsCursorVisible() const {
    return SDL_ShowCursor(-1) == SDL_ENABLE;
}

bool Window::IsCapturingMouse() const {
    return (SDL_GetWindowFlags(SDL_WINDOW) & SDL_WINDOW_MOUSE_CAPTURE) != 0;
}

bool Window::IsRelativeMouseMode() const {
    return SDL_GetRelativeMouseMode() == SDL_TRUE;
}

void* Window::GetNativeHandle() {
    return SDL_WINDOW;
}

void Window::ImGuiNewFrame() {
    ImGui_ImplSDL2_NewFrame(SDL_WINDOW);
}


#define MAP(sdlk, k) case SDLK_##sdlk: return Key::k

Key KeySymToKey(SDL_Keysym sym) {

    switch (sym.sym) {
        MAP(UNKNOWN, Unknown);

        MAP(a, A);
        MAP(b, B);
        MAP(c, C);
        MAP(d, D);
        MAP(e, E);
        MAP(f, F);
        MAP(g, G);
        MAP(h, H);
        MAP(i, I);
        MAP(j, J);
        MAP(k, K);
        MAP(l, L);
        MAP(m, M);
        MAP(n, N);
        MAP(o, O);
        MAP(p, P);
        MAP(q, Q);
        MAP(r, R);
        MAP(s, S);
        MAP(t, T);
        MAP(u, U);
        MAP(v, V);
        MAP(w, W);
        MAP(x, X);
        MAP(y, Y);
        MAP(z, Z);

        MAP(0, Num0);
        MAP(1, Num1);
        MAP(2, Num2);
        MAP(3, Num3);
        MAP(4, Num4);
        MAP(5, Num5);
        MAP(6, Num6);
        MAP(7, Num7);
        MAP(8, Num8);
        MAP(9, Num9);

        MAP(RETURN, Enter);
        MAP(ESCAPE, Escape);
        MAP(BACKSPACE, Backspace);
        MAP(TAB, Tab);
        MAP(SPACE, Space);

        MAP(LSHIFT, LeftShift);
        MAP(LALT, LeftAlt);
        MAP(LCTRL, LeftCtrl);
        MAP(RSHIFT, RightShift);
        MAP(RALT, RightAlt);
        MAP(RCTRL, RightCtrl);

        MAP(LEFTBRACKET, LeftBracket);
        MAP(RIGHTBRACKET, RightBracket);
        MAP(SEMICOLON, Semicolon);
        MAP(COMMA, Comma);
        MAP(PERIOD, Period);
        MAP(QUOTE, Quote);
        MAP(SLASH, Slash);
        MAP(BACKSLASH, Backslash);
        MAP(BACKQUOTE, Grave);
        MAP(EQUALS, Equals);
        MAP(MINUS, Hyphen);

        MAP(PAGEUP, PageUp);
        MAP(PAGEDOWN, PageDown);
        MAP(END, End);
        MAP(HOME, Home);
        MAP(INSERT, Insert);
        MAP(DELETE, Delete);

        MAP(KP_MEMADD, NumpadAdd);
        MAP(KP_MEMSUBTRACT, NumpadSub);
        MAP(KP_MEMMULTIPLY, NumpadMul);
        MAP(KP_MEMDIVIDE, NumpadDiv);

        MAP(LEFT, Left);
        MAP(RIGHT, Right);
        MAP(UP, Up);
        MAP(DOWN, Down);

        MAP(KP_0, Numpad0);
        MAP(KP_1, Numpad1);
        MAP(KP_2, Numpad2);
        MAP(KP_3, Numpad3);
        MAP(KP_4, Numpad4);
        MAP(KP_5, Numpad5);
        MAP(KP_6, Numpad6);
        MAP(KP_7, Numpad7);
        MAP(KP_8, Numpad8);
        MAP(KP_9, Numpad9);

        MAP(F1, F1);
        MAP(F2, F2);
        MAP(F3, F3);
        MAP(F4, F4);
        MAP(F5, F5);
        MAP(F6, F6);
        MAP(F7, F7);
        MAP(F8, F8);
        MAP(F9, F9);
        MAP(F10, F10);
        MAP(F11, F11);
        MAP(F12, F12);

        MAP(PAUSE, Pause);
        MAP(MENU, Menu);
    }

    return Key::Unknown;
}

}
