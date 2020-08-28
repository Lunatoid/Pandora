#pragma once

#include "Pandora/Core/Data/StringView.h"
#include "Pandora/Core/Resources/ResourceCatalog.h"
#include "Pandora/Core/Event.h"

#include "Pandora/Graphics/Camera.h"

// In the future this would get switched with a more proper scene system complete with scene graph.
namespace pd {

class App;
class SceneManager;
class Renderer;
class Window;
class InputManager;

class Scene {
public:
    Scene(App* app, StringView name);
    virtual ~Scene() = default;
    
    /// <summary>
    /// Called when the scene manager switches to this scene.
    /// </summary>
    virtual void OnEnter() {}    
    /// <summary>
    /// Called when the scene manager switches from this scene.
    /// </summary>
    virtual void OnLeave() {}
    
    /// <summary>
    /// Called on each update frame.
    /// </summary>
    /// <param name="dt">The update delta time.</param>
    virtual void OnUpdate(f32 dt) {}
    
    /// <summary>
    /// Called on each render frame.
    /// </summary>
    /// <param name="dt">The render delta time.</param>
    virtual void OnRender(f32 dt) {}

#if !defined(PD_NO_IMGUI)    
    /// <summary>
    /// Called on each ImGui frame.
    /// </summary>
    virtual void OnImGui() {}
#endif
    
    /// <summary>
    /// Called when pumping application events.
    /// </summary>
    /// <param name="event">The event.</param>
    virtual void OnEvent(Event* event) {}
    
    /// <summary>
    /// Gets the name of the scene.
    /// </summary>
    /// <returns>The name of the scene.</returns>
    StringView GetName();
    
    /// <summary>
    /// Gets the camera of the scene.
    /// </summary>
    /// <returns>The camera of the scene.</returns>
    Camera& GetCamera();

protected:
    App* app = nullptr;
    Ref<Renderer> renderer;
    ResourceCatalog& resources;
    Window& window;
    InputManager& input;
    SceneManager& sceneManager;

    String name;
    Camera cam;
};

}
