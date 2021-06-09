#pragma once

#include "Pandora/Core.h"
#include "Pandora/Graphics.h"
#include "Pandora/Audio.h"

namespace pd {


class App {
public:
    App(int argc, char** argv, VideoBackend backend, StringView title, Vec2i size, u32 style = WindowDefault);
    virtual ~App();

    void Run();

    void Quit();
    bool IsRunning() const;

    /**
     * \brief Pushes an event.
     * 
     * \param event The event.
     */
    void PushEvent(Event* event);

    f32 updateTimescale = 1.0f;
    f32 renderTimescale = 1.0f;

    /**
     * \return The mouse position relative to the window. (0, 0) is bottom left.
     * The position is normalized from -1.0 to 1.0.
     */
    Vec2 GetNormalizedMousePos();

    /**
     * \return The mouse position relative to the window. (0, 0) is bottom left.
     */
    Vec2 GetMousePos() const;

    /**
     * \return The window.
     */
    Window& GetWindow();

    /**
     * \return The input manager.
     */
    InputManager& GetInputManager();

    /**
     * \param type The reference type.
     * \return A new reference to the renderer.
     */
    Ref<Renderer> GetRenderer(RefType type = RefType::Strong);

protected:
    // Overload these
    virtual void OnQuit() {}
    virtual void OnUpdate(f32 dt) {}
    virtual void OnRender(f32 dt) {}

#if !defined(PD_NO_IMGUI)
    virtual void OnImGui() {}
#endif

    virtual void OnEvent(Event* event) {}

    Array<String> args;

    Window window;
    InputManager input;
    VideoAPI* video = nullptr;
    AudioAPI* audio = nullptr;
    Ref<Renderer> renderer;
    ResourceCatalog& catalog;

private:
    void OnUpdateInternal(f32 dt);
    void OnRenderInternal(f32 dt);

#if !defined(PD_NO_IMGUI)
    void OnImGuiInternal();
    void LoadImGuiTheme();

    Array<byte> fontData;
    ImFont* imguiFont = nullptr;
#endif

    void OnEventInternal(Event* event);

    f32 rawDelta = 0.0f;
    bool isRunning = false;
};

pd::App* CreateApp(int argc, char** argv);

}
