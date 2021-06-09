#pragma once

#include "Pandora/Core/Data/StringView.h"
#include "Pandora/Core/Data/Array.h"
#include "Pandora/Core/Data/Optional.h"
#include "Pandora/Core/Math/VectorInt.h"
#include "Pandora/Core/Window/WindowEvent.h"
#include "Pandora/Core/VideoBackend.h"

namespace pd {

enum WindowStyle : u32 {
    WindowNone = 0,
    WindowResizable = 1 << 1,
    WindowFullscreen = 1 << 2,
    WindowDefault = WindowNone
};

class Window {
public:
    Window();
    ~Window();

    /**
     * \brief Destroys the window.
     * Gets called on destruction.
     */
    void Delete();

    /**
     * \brief Opens a new window.
     * 
     * \param title The window title.
     * \param size The window size.
     * \param style The window size.
     * \param backend The backend for this window.
     */
    void Open(StringView title, Vec2i size, u32 style = WindowDefault, VideoBackend backend = VideoBackend::Default);

    /**
     * \brief Pumps the window events.
     */
    void HandleEvents();

    /**
     * \brief Gets the first event in the queue.
     * 
     * \param out The output for the event.
     * \return If ab event got pulled from the queue.
     */
    bool PollEvent(WindowEvent* out);

    /**
     * \brief Locks or unlocks the mouse within the window.
     * 
     * \param shouldCapture Whether it should lock or unlock.
     */
    void CaptureMouse(bool shouldCapture);

    /**
     * \brief Sets the mouse to a positition relative to the window.
     * 
     * \param position The relative position.
     */
    void SetMousePosition(Vec2i position);

    /**
     * \brief Sets the mouse mode to relative.
     * The mouse will be locked to the center of the screen and all mouse
     * movement events will rely on relative motion events.
     * 
     * \param enabled Whether to enable or disable relative mouse mode.
     */
    void SetRelativeMouseMode(bool enabled);

    /**
     * \return The size of the window.
     */
    Vec2i GetSize();

    /**
     * \return Whether or not the window is open.
     */
    bool IsOpen() const;

    /**
     * \return Whether or not the window is fullscreen.
     */
    bool IsFullscreen() const;

    /**
     * \brief Shows or hides the window.
     * 
     * \param isVisible Whether or not the window should be visible.
     */
    void SetWindowVisible(bool isVisible);

    /**
     * \brief Sets the visibility of the cursor.
     * 
     * \param isVisible Whether or not it should be visible.
     */
    void SetCursorVisible(bool isVisible);

    /**
     * \brief Sets the window to fullscreen or windowed.
     * 
     * \param isFullscreen Whether or not it should be fullscreen.
     */
    void SetFullscreen(bool isFullscreen);

    /**
     * \return Whether or not the cursor is visible.
     */
    bool IsCursorVisible() const;

    /**
     * \return Whether or not the window is capturing the mouse.
     */
    bool IsCapturingMouse() const;

    /**
     * \return Whether or not the window is in relative mouse mode.
     */
    bool IsRelativeMouseMode() const;

    /**
     * \return The native window handle.
     */
    void* GetNativeHandle();

    /**
     * \return The backend the window was created with.
     */
    VideoBackend GetBackend() const;

#if !defined(PD_NO_IMGUI)
    /**
     * \brief Starts a new ImGui frame.
     */
    void ImGuiNewFrame();
#endif

private:
    Vec2i windowSize;

    bool isOpen = false;
    VideoBackend backend;

    Array<WindowEvent> events;

    void* nativeData = nullptr;
};

}
