#pragma once

#include "Pandora/Core/Data/StringView.h"
#include "Pandora/Core/Data/Array.h"
#include "Pandora/Core/Data/Optional.h"

#include "Pandora/Core/Math/VectorInt.h"

#include "Pandora/Core/Window/WindowEvent.h"

namespace pd {

enum WindowStyle : u32 {
    WindowNone = 0,
    WindowResizable = 1 << 1,
    WindowClosable = 1 << 2,
    WindowFullscreen = 1 << 3,
    WindowTitlebar = 1 << 4,
    WindowDefault = WindowClosable | WindowTitlebar
};

class Window {
public:
    Window();
    ~Window();

    void Delete();
    
    /// <summary>
    /// Opens a new Window.
    /// </summary>
    /// <param name="title">The title.</param>
    /// <param name="size">The size.</param>
    /// <param name="style">The style.</param>
    void Open(StringView title, Vec2i size, u32 style = WindowDefault);
        
    /// <summary>
    /// Pumps the window events.
    /// </summary>
    void HandleEvents();
    
    /// <summary>
    /// Gets the first event in the queue.
    /// </summary>
    /// <param name="out">The output for the event.</param>
    /// <returns>Whether or not an event was polled.</returns>
    bool PollEvent(WindowEvent* out);
    
    /// <summary>
    /// Locks or unlocks the cursor from within the window.
    /// </summary>
    /// <param name="shouldCapture">Whether or not the mouse should be captured.</param>
    void CaptureMouse(bool shouldCapture);

    /// <summary>
    /// Sets the mouse to a position within the window.
    /// </summary>
    /// <param name="position">The new position.<param>
    void SetMousePosition(Vec2i position);

    /// <summary>
    /// Sets the mouse mode to relative.
    /// The mouse will be locked to the center of the screen and all mouse movement will rely on relative mouse events.
    /// </summary>
    /// <param name="enabled">Whether or not it should be enabled.</param>
    void SetRelativeMouseMode(bool enabled);

    /// <summary>
    /// Returns the size of the window.
    /// </summary>
    /// <returns>The size of the window.</returns>
    Vec2i GetSize();
    
    /// <summary>
    /// Returns whether or not the window is open.
    /// </summary>
    /// <returns>Whether or not the window is open.</returns>
    bool IsOpen() const;
        
    /// <summary>
    /// Returns whether or not the window is currently fullscreen.
    /// </summary>
    /// <returns><c>true</c> if fullscreen. <c>false</c> if not.</returns>
    bool IsFullscreen() const;

    /// <summary>
    /// Sets the visibility of the window.
    /// </summary>
    /// <param name="isVisible">Whether or not it should be visible.</param>
    void SetWindowVisible(bool isVisible);

    /// <summary>
    /// Sets the visibility of the cursor.
    /// </summary>
    /// <param name="isVisible">Whether or not it should be visible.</param>
    void SetCursorVisible(bool isVisible);

    /// <summary>
    /// Sets the screen to either windowed or fullscreen.
    /// </summary>
    /// <param name="isFullscreen"><c>true</c> for fullscreen. <c>false</c> for windowed.</param>
    /// <param name="size">The size of the fullscreen/windowed window.</param>
    void SetFullscreen(bool isFullscreen, Vec2i size);
        
    /// <summary>
    /// Returns whether or not the cursor is visible.
    /// </summary>
    /// <returns>Whether or not the cursor is visible.</returns>
    bool IsCursorVisible() const;
    
    /// <summary>
    /// Returns whether or not the window is capturing the mouse.
    /// </summary>
    /// <returns>Whether or not the window is capturing the mouse.</returns>
    bool IsCapturingMouse() const;
    
    /// <summary>
    /// Returns whether or not the window is in relative mouse mode.
    /// </summary>
    /// <returns>Whether or not the window is in relative mouse mode.</returns>
    bool IsRelativeMouseMode() const;

    /// <summary>
    /// Returns the native window handle.
    /// On Windows this is the <c>HWND</c>.
    /// </summary>
    /// <returns>The native window handle.</returns>
    void* GetNativeHandle();

#if !defined(PD_NO_IMGUI)
    /// <summary>
    /// Starts a new ImGui frame for the window implementation.
    /// </summary>
    void ImGuiNewFrame();
#endif

#if defined(PD_WINDOWS) && defined(PD_LIB)    
    /// <summary>
    /// Handles the Win32 window events. Used internally.
    /// </summary>
    /// <param name="message">The message.</param>
    /// <param name="lparam">The lparam.</param>
    /// <param name="wparam">The wparam.</param>
    void Win32InternalHandleEvent(u32 message, u64 lparam, i64 wparam);
#endif

private:
    Vec2i windowSize;

    bool isOpen = false;
    bool isFullscreen = false;
    bool isResizable = false;
    bool isCursorVisible = true;
    bool isCapturingCursor = false;
    bool isRelativeMouseMode = false;

    Array<WindowEvent> events;

    void* nativeData = nullptr;
};

}
