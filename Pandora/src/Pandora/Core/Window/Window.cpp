#include "Window.h"

#include "Pandora/Core/Assert.h"

#include "Pandora/Core/Data/String.h"

#include "Pandora/Core/Math/VectorInt.h"

#if defined(PD_WINDOWS)
#include <Windows.h>
#include <Windowsx.h>
#include <WinUser.h>

#if !defined(PD_NO_IMGUI)
#include "Pandora/Libs/ImGui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif
#endif

namespace pd {

const wchar* DEFAULT_WINDOW_NAME = L"Pandora Window";

#if defined(PD_WINDOWS)

#define NATIVE_DATA ((NativeData*)this->nativeData)

Key VKeyToKey(WPARAM vkey, LPARAM flags);

struct NativeData {
    HWND handle = nullptr;
    HCURSOR cursor = nullptr;

    bool isResizing = false;
    Vec2i lastResizeSize;

    wchar multiUtf16 = '\0';

    Vec2i lastMove;
    DWORD windowedStyle = 0;
};

Window::Window() {}

Window::~Window() {
    Delete();
}

void Window::Delete() {
    isOpen = false;

    if (nativeData && NATIVE_DATA->handle) {
        DestroyWindow(NATIVE_DATA->handle);
    }

    if (nativeData && NATIVE_DATA->cursor) {
        DestroyCursor(NATIVE_DATA->cursor);
    }

    if (nativeData) {
        pd::Delete((NativeData*)nativeData);
        nativeData = nullptr;
    }

    UnregisterClassW(DEFAULT_WINDOW_NAME, GetModuleHandleW(NULL));

#if !defined(PD_NO_IMGUI)
    if (ImGui::GetCurrentContext()) {
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }
#endif
}

void Window::Open(StringView title, Vec2i size, u32 style) {
    Delete();

    nativeData = (void*)New<NativeData>();

    isFullscreen = (style & WindowFullscreen) != 0;
    isResizable = (style & WindowResizable) != 0;
    isOpen = true;

    NATIVE_DATA->cursor = LoadCursorW(NULL, IDC_ARROW);

    WNDCLASSW wc = {};
    wc.lpfnWndProc = [](HWND handle, UINT message, WPARAM wparam, LPARAM lparam) -> LRESULT {
        if (message == WM_CREATE) {
            // Set the pd::Window as the user data
            SetWindowLongPtrW(handle, GWLP_USERDATA, (LONG_PTR)((CREATESTRUCT*)lparam)->lpCreateParams);
        }

        Window* window = (handle) ? (Window*)GetWindowLongPtrW(handle, GWLP_USERDATA) : NULL;

        if (window) {
#if !defined(PD_NO_IMGUI)
            // Update ImGui
            ImGui_ImplWin32_WndProcHandler(handle, message, wparam, lparam);
#endif
            window->Win32InternalHandleEvent(message, wparam, lparam);
        }

        if (message == WM_CLOSE) return 0;

        return DefWindowProcW(handle, message, wparam, lparam);
    };

    wc.hInstance = GetModuleHandleW(NULL);
    wc.lpszClassName = DEFAULT_WINDOW_NAME;
    
    RegisterClassW(&wc);

    HDC screenDC = GetDC(NULL);

    DWORD windowStyle = WS_VISIBLE;

    if (style == WindowNone) {
        windowStyle |= WS_POPUP;
    } else {
        if (style & WindowTitlebar) {
            windowStyle |= WS_CAPTION | WS_MINIMIZEBOX;
        }

        if (style & WindowResizable) {
            windowStyle |= WS_THICKFRAME | WS_MAXIMIZEBOX;
        }

        if (style & WindowClosable) {
            windowStyle |= WS_SYSMENU;
        }
    }

    NATIVE_DATA->windowedStyle = windowStyle;

    // Windows includes the size of the titlebar and such in the width and height
    // Make it so that the window size itself is as big as the requested size
    if (!isFullscreen) {
        RECT rect = {};
        rect.right = size.x;
        rect.bottom = size.y;

        AdjustWindowRect(&rect, windowStyle, false);

        size.x = rect.right - rect.left;
        size.y = rect.bottom - rect.top;
    }

    // Sometimes error 998 was thrown when we didn't use an intermediate variable for the titles
    wchar* wideTitle = title.ToWide(Allocator::Persistent);
    NATIVE_DATA->handle = CreateWindowExW(0, wc.lpszClassName, wideTitle, windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, size.x, size.y, NULL, NULL, wc.hInstance, NULL);
    Free(wideTitle);

    PD_ASSERT_D(NATIVE_DATA->handle != NULL, "failed to create window\n(%d): %s", GetLastError(), GetWin32LastErrorMessage());

    // Set the user-data to this window instance
    SetWindowLongPtrW(NATIVE_DATA->handle, GWLP_USERDATA, (LONG_PTR)this);

    // Set last handled size
    NATIVE_DATA->lastResizeSize = GetSize();

    if (isFullscreen) {
        SetFullscreen(true, size);
    }

#if !defined(PD_NO_IMGUI)
    ImGui::CreateContext();

    // Set up config BEFORE initializing backends
    ImGuiIO& io = ImGui::GetIO();
    io.RenderDrawListsFn = nullptr; // For some reason this was not always set to nullptr
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplWin32_Init(NATIVE_DATA->handle);


    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& imStyle = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        imStyle.WindowRounding = 0.0f;
        imStyle.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }
#endif
}

void Window::HandleEvents() {
    if (!IsOpen()) return;

    MSG msg;
    while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
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
    if (!IsOpen()) return;

    isCapturingCursor = shouldCapture;

    if (shouldCapture) {
        RECT rect;
        GetClientRect(NATIVE_DATA->handle, &rect);
        MapWindowPoints(NATIVE_DATA->handle, nullptr, (LPPOINT)&rect, 2);
        ClipCursor(&rect);
    } else {
        ClipCursor(NULL);
    }
}

void Window::SetMousePosition(Vec2i position) {
    if (!IsOpen()) return;

    POINT pt;
    pt.x = position.x;
    pt.y = position.y;
    ClientToScreen(NATIVE_DATA->handle, &pt);
    SetCursorPos(pt.x, pt.y);
}


void Window::SetRelativeMouseMode(bool enabled) {
    if (!IsOpen() || enabled == isRelativeMouseMode) return;

    isRelativeMouseMode = enabled;

    if (enabled) {
        // Remove all pending mouse move events
        for (int i = events.Count() - 1; i >= 0; i--) {
            if (events[i].type == WindowEventType::MouseMove) {
                events.Remove(i);
            }
        }
    }
}

Vec2i Window::GetSize() {
    if (!IsOpen()) return Vec2i(0, 0);

    RECT rect;
    GetClientRect(NATIVE_DATA->handle, &rect);

    return Vec2i(rect.right - rect.left, rect.bottom - rect.top);
}

bool Window::IsOpen() const {
    return isOpen;
}

bool Window::IsFullscreen() const {
    return isFullscreen;
}

#if defined(PD_LIB)

void Window::Win32InternalHandleEvent(u32 message, u64 wparam, i64 lparam) {
    if (!NATIVE_DATA) return;

    switch (message) {
        case WM_DESTROY: {
            // Even though we want to quit we're not going to call Delete()
            // because some other things might depend on the window
            PostQuitMessage(0);
            break;
        }

        case WM_CLOSE: {
            isOpen = false;

            WindowEvent event;
            event.type = WindowEventType::Close;

            events.Add(event);
            break;
        }

        case WM_SETCURSOR: {
            if (LOWORD(lparam) == HTCLIENT) {
                SetCursorVisible(isCursorVisible);
            }
        }

        case WM_SIZE: {
            if (wparam != SIZE_MINIMIZED && !NATIVE_DATA->isResizing && NATIVE_DATA->lastResizeSize != GetSize()) {
                NATIVE_DATA->lastResizeSize = GetSize();

                WindowEvent event;
                event.type = WindowEventType::Resize;
                event.resize.newSize = GetSize();

                events.Add(event);
            }
            break;
        }

        case WM_ENTERSIZEMOVE: {
            NATIVE_DATA->isResizing = true;
            break;
        }

        case WM_EXITSIZEMOVE: {
            NATIVE_DATA->isResizing = false;

            if (NATIVE_DATA->lastResizeSize != GetSize()) {
                NATIVE_DATA->lastResizeSize = GetSize();

                WindowEvent event;
                event.type = WindowEventType::Resize;
                event.resize.newSize = GetSize();

                events.Add(event);
            }
            break;
        }

        case WM_SETFOCUS: {
            WindowEvent event;
            event.type = WindowEventType::GainedFocus;

            events.Add(event);

            // Make sure we're still capturing the mouse
            if (isCapturingCursor) {
                CaptureMouse(isCapturingCursor);
            }

            break;
        }

        case WM_KILLFOCUS: {
            WindowEvent event;
            event.type = WindowEventType::LostFocus;

            events.Add(event);
            break;
        }

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN: {
#if !defined(PD_NO_IMGUI)
            if (ImGui::GetIO().WantCaptureKeyboard) return;
#endif

            Key targetKey = VKeyToKey(wparam, lparam);

            // Don't apply key repeat for KeyDown
            if ((HIWORD(lparam) & KF_REPEAT) == 0) {
                WindowEvent event;
                event.type = WindowEventType::KeyDown;
                event.keyPress.key = targetKey;
                event.keyPress.isDown = true;

                events.Add(event);
            }

            WindowEvent event;
            event.type = WindowEventType::KeyRepeat;
            event.keyRepeat.key = targetKey;
            event.keyRepeat.repeatCount = HIWORD(lparam) & KF_REPEAT;

            events.Add(event);
            break;
        }

        case WM_KEYUP:
        case WM_SYSKEYUP: {
#if !defined(PD_NO_IMGUI)
            if (ImGui::GetIO().WantCaptureKeyboard) return;
#endif

            WindowEvent event;
            event.type = WindowEventType::KeyUp;
            event.keyPress.key = VKeyToKey(wparam, lparam);
            event.keyPress.isDown = false;

            events.Add(event);
            break;
        }

        case WM_CHAR:
        case WM_SYSCHAR: {
#if !defined(PD_NO_IMGUI)
            if (ImGui::GetIO().WantCaptureKeyboard) return;
#endif

            u32 character = (u32)wparam;

            // Check if it's a multi-char UTF-16 character
            if (character >= 0xD800 && character <= 0xDBFF) {
                NATIVE_DATA->multiUtf16 = (wchar)character;
            } else {
                WindowEvent event;
                event.type = WindowEventType::TextDown;

                // Check if it's a multi-char UTF-16 character
                if (character >= 0xDC00 && character <= 0xDFFF) {
                    // Roundabout way of getting the codepoint...
                    wchar buffer[3];
                    buffer[0] = NATIVE_DATA->multiUtf16;
                    buffer[1] = (wchar)character;
                    buffer[2] = '\0';

                    uchar* utf8 = WideToUTF8(buffer);

                    GetNextCodepoint(utf8, &event.textDown.point);
                } else {
                    event.textDown.point = character;
                }

                // Get scancode and virtual key and convert it to a pd::Key
                UINT scancode = ((lparam & (0xFF << 16)) >> 16);
                UINT vkey = MapVirtualKeyW(scancode, MAPVK_VSC_TO_VK);
                event.textDown.key = VKeyToKey(vkey, 0);

                events.Add(event);
            }
            break;
        }

        case WM_LBUTTONDOWN: {
#if !defined(PD_NO_IMGUI)
            if (ImGui::GetIO().WantCaptureMouse) return;
#endif

            WindowEvent event;
            event.type = WindowEventType::MouseButtonDown;
            event.mousePress.button = MouseButton::Left;
            event.mousePress.position = Vec2i(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            event.mousePress.isDown = true;

            events.Add(event);
            break;
        }

        case WM_LBUTTONUP: {
#if !defined(PD_NO_IMGUI)
            if (ImGui::GetIO().WantCaptureMouse) return;
#endif

            WindowEvent event;
            event.type = WindowEventType::MouseButtonDown;
            event.mousePress.button = MouseButton::Left;
            event.mousePress.position = Vec2i(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            event.mousePress.isDown = false;

            events.Add(event);
            break;
        }

        case WM_RBUTTONDOWN: {
#if !defined(PD_NO_IMGUI)
            if (ImGui::GetIO().WantCaptureMouse) return;
#endif

            WindowEvent event;
            event.type = WindowEventType::MouseButtonDown;
            event.mousePress.button = MouseButton::Right;
            event.mousePress.position = Vec2i(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            event.mousePress.isDown = true;

            events.Add(event);
            break;
        }

        case WM_RBUTTONUP: {
#if !defined(PD_NO_IMGUI)
            if (ImGui::GetIO().WantCaptureMouse) return;
#endif

            WindowEvent event;
            event.type = WindowEventType::MouseButtonDown;
            event.mousePress.button = MouseButton::Right;
            event.mousePress.position = Vec2i(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            event.mousePress.isDown = false;

            events.Add(event);
            break;
        }

        case WM_MBUTTONDOWN: {
#if !defined(PD_NO_IMGUI)
            if (ImGui::GetIO().WantCaptureMouse) return;
#endif

            WindowEvent event;
            event.type = WindowEventType::MouseButtonDown;
            event.mousePress.button = MouseButton::Middle;
            event.mousePress.position = Vec2i(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            event.mousePress.isDown = true;

            events.Add(event);
            break;
        }

        case WM_MBUTTONUP: {
#if !defined(PD_NO_IMGUI)
            if (ImGui::GetIO().WantCaptureMouse) return;
#endif

            WindowEvent event;
            event.type = WindowEventType::MouseButtonDown;
            event.mousePress.button = MouseButton::Middle;
            event.mousePress.position = Vec2i(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            event.mousePress.isDown = false;

            events.Add(event);
            break;
        }

        case WM_MOUSEWHEEL: {
#if !defined(PD_NO_IMGUI)
            if (ImGui::GetIO().WantCaptureMouse) return;
#endif

            WindowEvent event;
            event.type = WindowEventType::MouseWheel;

            i16 delta = (i16)HIWORD(wparam);

            event.mouseWheel.y = (f32)delta / (f32)WHEEL_DELTA;

            events.Add(event);
            break;
        }

        case WM_MOUSEHWHEEL: {
#if !defined(PD_NO_IMGUI)
            if (ImGui::GetIO().WantCaptureMouse) return;
#endif

            WindowEvent event;
            event.type = WindowEventType::MouseWheel;

            i16 delta = (i16)HIWORD(wparam);

            event.mouseWheel.x = (f32)delta / (f32)WHEEL_DELTA;

            events.Add(event);
            break;
        }

        case WM_MOUSEMOVE: {
#if !defined(PD_NO_IMGUI)
            if (ImGui::GetIO().WantCaptureMouse) return;
#endif

            WindowEvent event;
            event.type = WindowEventType::MouseMove;

            Vec2i position;
            position.x = (int)GET_X_LPARAM(lparam);
            position.y = (int)GET_Y_LPARAM(lparam);

            event.mouseMove.position = position;
            event.mouseMove.delta = position - NATIVE_DATA->lastMove;

            events.Add(event);

            if (isRelativeMouseMode) {
                Vec2i center = Vec2i(GetSize().x / 2, GetSize().y / 2);

                NATIVE_DATA->lastMove = center;
                SetMousePosition(center);
            } else {
                NATIVE_DATA->lastMove = position;
            }

            break;
        }
    }
}

#endif

void Window::SetWindowVisible(bool isVisible) {
    if (!IsOpen()) return;

    ShowWindow(NATIVE_DATA->handle, (isVisible) ? SW_SHOW : SW_HIDE);
}

void Window::SetCursorVisible(bool isVisible) {
    if (!IsOpen()) return;

    isCursorVisible = isVisible;

    SetCursor((isCursorVisible) ? NATIVE_DATA->cursor : NULL);
}


void Window::SetFullscreen(bool isFullscreen, Vec2i size) {
    if (!IsOpen()) return;

    this->isFullscreen = isFullscreen;

    if (isFullscreen) {
        // We need to reapply the display settings for fullscreen
        DEVMODEW devMode = {};
        devMode.dmSize = sizeof(devMode);
        devMode.dmPelsWidth = size.x;
        devMode.dmPelsHeight = size.y;
        devMode.dmBitsPerPel = 32;
        devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

        PD_ASSERT_D(!ChangeDisplaySettingsW(&devMode, CDS_FULLSCREEN), "failed to change the window display mode for fullscreen");

        // Make sure we style ourselves as a fullscreen window
        SetWindowLongW(NATIVE_DATA->handle, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
        SetWindowLongW(NATIVE_DATA->handle, GWL_EXSTYLE, WS_EX_APPWINDOW);

        // Now actually become full
        SetWindowPos(NATIVE_DATA->handle, HWND_TOP, 0, 0, size.x, size.y, SWP_FRAMECHANGED);
        ShowWindow(NATIVE_DATA->handle, SW_SHOW);
    } else {
        // Revert to our original size
        SetWindowLongW(NATIVE_DATA->handle, GWL_STYLE, NATIVE_DATA->windowedStyle);

        // Get desktop size
        Vec2i screenSize;
        screenSize.x = GetSystemMetrics(SM_CXSCREEN);
        screenSize.y = GetSystemMetrics(SM_CYSCREEN);

        SetWindowPos(NATIVE_DATA->handle, HWND_TOP,
            (screenSize.x - size.x) / 2, // Center
            (screenSize.y - size.y) / 2, // Center
            size.x, size.y, SWP_FRAMECHANGED);

    }
}

bool Window::IsCursorVisible() const {
    return isCursorVisible;
}

bool Window::IsCapturingMouse() const {
    return isCapturingCursor;
}

bool Window::IsRelativeMouseMode() const {
    return isRelativeMouseMode;
}

void* Window::GetNativeHandle() {
    return (NATIVE_DATA) ? NATIVE_DATA->handle : nullptr;
}

#if !defined(PD_NO_IMGUI)
void Window::ImGuiNewFrame() {
    ImGui_ImplWin32_NewFrame();
}
#endif

Key VKeyToKey(WPARAM vkey, LPARAM flags) {
    Key key = Key::Unknown;

    switch (vkey) {
        case 'A':            key = Key::A;            break;
        case 'B':            key = Key::B;            break;
        case 'C':            key = Key::C;            break;
        case 'D':            key = Key::D;            break;
        case 'E':            key = Key::E;            break;
        case 'F':            key = Key::F;            break;
        case 'G':            key = Key::G;            break;
        case 'H':            key = Key::H;            break;
        case 'I':            key = Key::I;            break;
        case 'J':            key = Key::J;            break;
        case 'K':            key = Key::K;            break;
        case 'L':            key = Key::L;            break;
        case 'M':            key = Key::M;            break;
        case 'N':            key = Key::N;            break;
        case 'O':            key = Key::O;            break;
        case 'P':            key = Key::P;            break;
        case 'Q':            key = Key::Q;            break;
        case 'R':            key = Key::R;            break;
        case 'S':            key = Key::S;            break;
        case 'T':            key = Key::T;            break;
        case 'U':            key = Key::U;            break;
        case 'V':            key = Key::V;            break;
        case 'W':            key = Key::W;            break;
        case 'X':            key = Key::X;            break;
        case 'Y':            key = Key::Y;            break;
        case 'Z':            key = Key::Z;            break;

        case '0':            key = Key::Num0;         break;
        case '1':            key = Key::Num1;         break;
        case '2':            key = Key::Num2;         break;
        case '3':            key = Key::Num3;         break;
        case '4':            key = Key::Num4;         break;
        case '5':            key = Key::Num5;         break;
        case '6':            key = Key::Num6;         break;
        case '7':            key = Key::Num7;         break;
        case '8':            key = Key::Num8;         break;
        case '9':            key = Key::Num9;         break;

        case VK_RETURN:      key = Key::Enter;        break;
        case VK_ESCAPE:      key = Key::Escape;       break;
        case VK_BACK:        key = Key::Backspace;    break;
        case VK_TAB:         key = Key::Tab;          break;
        case VK_SPACE:       key = Key::Space;        break;

        case VK_CONTROL: {
            key = (HIWORD(flags) & KF_EXTENDED) ? Key::RightCtrl : Key::LeftCtrl;
            break;
        }

        case VK_MENU: {
            key = (HIWORD(flags) & KF_EXTENDED) ? Key::RightAlt : Key::LeftAlt;
            break;
        }

        case VK_SHIFT: {
            UINT leftShift = MapVirtualKeyW(VK_LSHIFT, MAPVK_VK_TO_VSC);
            UINT scancode = ((flags & (0xFF << 16)) >> 16);

            key = (scancode == leftShift) ? Key::LeftShift : Key::RightShift;
            break;
        }

        case VK_LWIN:        key = Key::LeftOS;       break;
        case VK_RWIN:        key = Key::RightOS;      break;

        case VK_OEM_4:       key = Key::LeftBracket;  break;
        case VK_OEM_6:       key = Key::RightBracket; break;
        case VK_OEM_1:       key = Key::Semicolon;    break;
        case VK_OEM_COMMA:   key = Key::Comma;        break;
        case VK_OEM_PERIOD:  key = Key::Period;       break;
        case VK_OEM_7:       key = Key::Quote;        break;
        case VK_OEM_2:       key = Key::Slash;        break;
        case VK_OEM_5:       key = Key::Backslash;    break;
        case VK_OEM_3:       key = Key::Tilde;        break;
        case VK_OEM_PLUS:    key = Key::Equal;        break;
        case VK_OEM_MINUS:   key = Key::Dash;         break;

        case VK_PRIOR:       key = Key::PageUp;       break;
        case VK_NEXT:        key = Key::PageDown;     break;
        case VK_END:         key = Key::End;          break;
        case VK_HOME:        key = Key::Home;         break;
        case VK_INSERT:      key = Key::Insert;       break;
        case VK_DELETE:      key = Key::Delete;       break;

        case VK_ADD:         key = Key::KeypadAdd;    break;
        case VK_SUBTRACT:    key = Key::KeypadSub;    break;
        case VK_MULTIPLY:    key = Key::KeypadMul;    break;
        case VK_DIVIDE:      key = Key::KeypadDiv;    break;

        case VK_LEFT:        key = Key::Left;         break;
        case VK_RIGHT:       key = Key::Right;        break;
        case VK_UP:          key = Key::Up;           break;
        case VK_DOWN:        key = Key::Down;         break;

        case VK_NUMPAD0:     key = Key::Numpad0;      break;
        case VK_NUMPAD1:     key = Key::Numpad1;      break;
        case VK_NUMPAD2:     key = Key::Numpad2;      break;
        case VK_NUMPAD3:     key = Key::Numpad3;      break;
        case VK_NUMPAD4:     key = Key::Numpad4;      break;
        case VK_NUMPAD5:     key = Key::Numpad5;      break;
        case VK_NUMPAD6:     key = Key::Numpad6;      break;
        case VK_NUMPAD7:     key = Key::Numpad7;      break;
        case VK_NUMPAD8:     key = Key::Numpad8;      break;
        case VK_NUMPAD9:     key = Key::Numpad9;      break;

        case VK_F1:          key = Key::F1;           break;
        case VK_F2:          key = Key::F2;           break;
        case VK_F3:          key = Key::F3;           break;
        case VK_F4:          key = Key::F4;           break;
        case VK_F5:          key = Key::F5;           break;
        case VK_F6:          key = Key::F6;           break;
        case VK_F7:          key = Key::F7;           break;
        case VK_F8:          key = Key::F8;           break;
        case VK_F9:          key = Key::F9;           break;
        case VK_F10:         key = Key::F10;          break;
        case VK_F11:         key = Key::F11;          break;
        case VK_F12:         key = Key::F12;          break;
        case VK_F13:         key = Key::F13;          break;
        case VK_F14:         key = Key::F14;          break;
        case VK_F15:         key = Key::F15;          break;

        case VK_PAUSE:       key = Key::Pause;        break;
        case VK_APPS:        key = Key::Menu;         break;
    }

    return key;
}

#endif

}
