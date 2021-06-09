#include "Input.h"

#include <SDL2/SDL.h>

namespace pd {

SDL_Keycode KeyToKeySym(Key key);

bool IsKeyDown(Key key) {
    const u8* keys = SDL_GetKeyboardState(NULL);
    return keys[SDL_GetScancodeFromKey(KeyToKeySym(key))] == 1;
}

bool IsMouseDown(MouseButton button) {
    u32 buttons = SDL_GetMouseState(NULL, NULL);

    int mask = 0;
    switch (button) {
        case MouseButton::Left:
            mask = SDL_BUTTON(SDL_BUTTON_LEFT);
            break;

        case MouseButton::Middle:
            mask = SDL_BUTTON(SDL_BUTTON_MIDDLE);
            break;

        case MouseButton::Right:
            mask = SDL_BUTTON(SDL_BUTTON_RIGHT);
            break;
    }

    return buttons & mask;
}

Vec2i GetScreenMousePosition() {
    Vec2i pos;
    SDL_GetMouseState(&pos.x, &pos.y);
    return pos;
}

#define MAP(sdlk, k) case Key::k: return SDLK_##sdlk;

SDL_Keycode KeyToKeySym(Key key) {

    switch (key) {
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

    switch (key) {
        case Key::LeftCtrl: return KMOD_LCTRL;
        case Key::LeftAlt: return KMOD_LALT;
        case Key::LeftShift: return KMOD_LSHIFT;

        case Key::RightCtrl: return KMOD_RCTRL;
        case Key::RightAlt: return KMOD_RALT;
        case Key::RightShift: return KMOD_RSHIFT;
    }

    return SDLK_UNKNOWN;
}

}
