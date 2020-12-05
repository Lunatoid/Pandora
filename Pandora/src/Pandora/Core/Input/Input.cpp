#include "Input.h"

#if defined(PD_WINDOWS)
#include <Windows.h>
#endif

namespace pd {

#if defined(PD_WINDOWS)

int VKeyToKey(Key key) {
    int vkey = 0;

    switch (key) {
        case Key::A:            vkey = 'A';           break;
        case Key::B:            vkey = 'B';           break;
        case Key::C:            vkey = 'C';           break;
        case Key::D:            vkey = 'D';           break;
        case Key::E:            vkey = 'E';           break;
        case Key::F:            vkey = 'F';           break;
        case Key::G:            vkey = 'G';           break;
        case Key::H:            vkey = 'H';           break;
        case Key::I:            vkey = 'I';           break;
        case Key::J:            vkey = 'J';           break;
        case Key::K:            vkey = 'K';           break;
        case Key::L:            vkey = 'L';           break;
        case Key::M:            vkey = 'M';           break;
        case Key::N:            vkey = 'N';           break;
        case Key::O:            vkey = 'O';           break;
        case Key::P:            vkey = 'P';           break;
        case Key::Q:            vkey = 'Q';           break;
        case Key::R:            vkey = 'R';           break;
        case Key::S:            vkey = 'S';           break;
        case Key::T:            vkey = 'T';           break;
        case Key::U:            vkey = 'U';           break;
        case Key::V:            vkey = 'V';           break;
        case Key::W:            vkey = 'W';           break;
        case Key::X:            vkey = 'X';           break;
        case Key::Y:            vkey = 'Y';           break;
        case Key::Z:            vkey = 'Z';           break;

        case Key::Num0:         vkey = '0';           break;
        case Key::Num1:         vkey = '1';           break;
        case Key::Num2:         vkey = '2';           break;
        case Key::Num3:         vkey = '3';           break;
        case Key::Num4:         vkey = '4';           break;
        case Key::Num5:         vkey = '5';           break;
        case Key::Num6:         vkey = '6';           break;
        case Key::Num7:         vkey = '7';           break;
        case Key::Num8:         vkey = '8';           break;
        case Key::Num9:         vkey = '9';           break;

        case Key::Enter:        vkey = VK_RETURN;     break;
        case Key::Escape:       vkey = VK_ESCAPE;     break;
        case Key::Backspace:    vkey = VK_BACK;       break;
        case Key::Tab:          vkey = VK_TAB;        break;
        case Key::Space:        vkey = VK_SPACE;      break;

        case Key::LeftCtrl:     vkey = VK_LCONTROL;   break;
        case Key::LeftAlt:      vkey = VK_LMENU;      break;
        case Key::LeftOS:       vkey = VK_LWIN;       break;
        case Key::LeftShift:    vkey = VK_LSHIFT;     break;
        case Key::RightCtrl:    vkey = VK_RCONTROL;   break;
        case Key::RightAlt:     vkey = VK_RMENU;      break;
        case Key::RightOS:      vkey = VK_RWIN;       break;
        case Key::RightShift:   vkey = VK_RSHIFT;     break;

        case Key::LeftBracket:  vkey = VK_OEM_4;      break;
        case Key::RightBracket: vkey = VK_OEM_6;      break;
        case Key::Semicolon:    vkey = VK_OEM_1;      break;
        case Key::Comma:        vkey = VK_OEM_COMMA;  break;
        case Key::Period:       vkey = VK_OEM_PERIOD; break;
        case Key::Quote:        vkey = VK_OEM_7;      break;
        case Key::Slash:        vkey = VK_OEM_2;      break;
        case Key::Backslash:    vkey = VK_OEM_5;      break;
        case Key::Tilde:        vkey = VK_OEM_3;      break;
        case Key::Equal:        vkey = VK_OEM_PLUS;   break;
        case Key::Dash:         vkey = VK_OEM_MINUS;  break;

        case Key::PageUp:       vkey = VK_PRIOR;      break;
        case Key::PageDown:     vkey = VK_NEXT;       break;
        case Key::End:          vkey = VK_END;        break;
        case Key::Home:         vkey = VK_HOME;       break;
        case Key::Insert:       vkey = VK_INSERT;     break;
        case Key::Delete:       vkey = VK_DELETE;     break;

        case Key::NumpadAdd:    vkey = VK_ADD;        break;
        case Key::NumpadSub:    vkey = VK_SUBTRACT;   break;
        case Key::NumpadMul:    vkey = VK_MULTIPLY;   break;
        case Key::NumpadDiv:    vkey = VK_DIVIDE;     break;

        case Key::Left:         vkey = VK_LEFT;       break;
        case Key::Right:        vkey = VK_RIGHT;      break;
        case Key::Up:           vkey = VK_UP;         break;
        case Key::Down:         vkey = VK_DOWN;       break;

        case Key::Numpad0:      vkey = VK_NUMPAD0;    break;
        case Key::Numpad1:      vkey = VK_NUMPAD1;    break;
        case Key::Numpad2:      vkey = VK_NUMPAD2;    break;
        case Key::Numpad3:      vkey = VK_NUMPAD3;    break;
        case Key::Numpad4:      vkey = VK_NUMPAD4;    break;
        case Key::Numpad5:      vkey = VK_NUMPAD5;    break;
        case Key::Numpad6:      vkey = VK_NUMPAD6;    break;
        case Key::Numpad7:      vkey = VK_NUMPAD7;    break;
        case Key::Numpad8:      vkey = VK_NUMPAD8;    break;
        case Key::Numpad9:      vkey = VK_NUMPAD9;    break;

        case Key::F1:           vkey = VK_F1;         break;
        case Key::F2:           vkey = VK_F2;         break;
        case Key::F3:           vkey = VK_F3;         break;
        case Key::F4:           vkey = VK_F4;         break;
        case Key::F5:           vkey = VK_F5;         break;
        case Key::F6:           vkey = VK_F6;         break;
        case Key::F7:           vkey = VK_F7;         break;
        case Key::F8:           vkey = VK_F8;         break;
        case Key::F9:           vkey = VK_F9;         break;
        case Key::F10:          vkey = VK_F10;        break;
        case Key::F11:          vkey = VK_F11;        break;
        case Key::F12:          vkey = VK_F12;        break;
        case Key::F13:          vkey = VK_F13;        break;
        case Key::F14:          vkey = VK_F14;        break;
        case Key::F15:          vkey = VK_F15;        break;

        case Key::Pause:        vkey = VK_PAUSE;      break;
        case Key::Menu:         vkey = VK_APPS;       break;
    }

    return vkey;
}

#endif

bool IsKeyDown(Key key) {
#if defined(PD_WINDOWS)
    return (GetAsyncKeyState(VKeyToKey(key)) & 0x8000) != 0;
#else
#error Implement me!
#endif
}

bool IsMouseDown(MouseButton button) {
#if defined(PD_WINDOWS)
    int vkey = 0;

    switch (button) {
        case MouseButton::Left:   vkey = GetSystemMetrics(SM_SWAPBUTTON) ? VK_RBUTTON : VK_LBUTTON; break;
        case MouseButton::Right:  vkey = GetSystemMetrics(SM_SWAPBUTTON) ? VK_LBUTTON : VK_RBUTTON; break;
        case MouseButton::Middle: vkey = vkey = VK_MBUTTON;
    }

    return (GetAsyncKeyState(vkey) & 0x8000) != 0;
#else
#error Implement me!
#endif
}

Vec2i GetScreenMousePosition() {
#if defined(PD_WINDOWS)
    POINT point;
    GetCursorPos(&point);
    return Vec2i((int)point.x, (int)point.y);
#else
#error Implement me!
#endif
}

void SetScreenMousePosition(Vec2i position) {
#if defined(PD_WINDOWS)
    SetCursorPos(position.x, position.y);
#else
#error Implement me!
#endif
}

}
