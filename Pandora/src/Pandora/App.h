#pragma once

#include "Pandora/Core.h"
#include "Pandora/Graphics.h"
#include "Pandora/Audio.h"
#include "Pandora/Scene.h"

namespace pd {

class App {
public:
    App(int argc, char** argv, VideoBackend backend, StringView title, Vec2i size, u32 style = WindowDefault);
    virtual ~App();

    void Run();

    void Quit();
    bool IsRunning() const;

    /// <summary>
    /// Pushes the event.
    /// </summary>
    /// <param name="event">The event.</param>
    void PushEvent(Event* event);

    f32 updateTimescale = 1.0f;
    f32 renderTimescale = 1.0f;
    
    /// <summary>
    /// Sets a fixed framerate for the update and render loop.
    /// </summary>
    /// <param name="isOn">Whether or not to use a fixed framerate.</param>
    /// <param name="fps">The FPS.</param>
    void SetFixedFramerate(bool isOn, f32 fps = 0.0f);

    /// <summary>
    /// Returns the mouse position relative to the window. Position is normalized from -1 to 1.
    /// </summary>
    /// <returns>The mouse position relative to the window. Position is normalized from -1 to 1.</returns>
    Vec2 GetNormalizedMousePos();
    
    /// <summary>
    /// Returns the mouse position relative to the window in pixels.
    /// </summary>
    /// <returns>The mouse position relative to the window in pixels.</returns>
    Vec2i GetMousePosition() const;
    
    /// <summary>
    /// Returns the window.
    /// </summary>
    /// <returns>The window.</returns>
    Window& GetWindow();
    
    /// <summary>
    /// Returns the input manager.
    /// </summary>
    /// <returns>The input manager.</returns>
    InputManager& GetInputManager();
    
    /// <summary>
    /// Returns a reference to the renderer.
    /// </summary>
    /// <param name="type">The reference type.</param>
    /// <returns>The renderer.</returns>
    Ref<Renderer> GetRenderer(RefType type = RefType::Strong);
        
    /// <summary>
    /// Returns the scene manager.
    /// </summary>
    /// <returns>The scene manager.</returns>
    SceneManager& GetSceneManager();
    
protected:
    // Overload these
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
    SceneManager sceneManager;

private:
    void OnUpdateInternal(f32 dt);
    void OnRenderInternal(f32 dt);

#if !defined(PD_NO_IMGUI)
    void OnImGuiInternal();
#endif

    void OnEventInternal(Event* event);
    
    f32 rawDelta = 0.0f;
    bool isRunning = false;

    bool fixedFramerate = false;
    f32 fixedFps = 0.0f;

    Vec2i mousePos;
};

pd::App* CreateApp(int argc, char** argv);

}
