#pragma once

#include "Pandora/Core/Data/StringView.h"
#include "Pandora/Core/Resources/ResourceCatalog.h"
#include "Pandora/Core/Event.h"

#include "Pandora/Graphics/Camera.h"

class Game;

namespace pd {

class SceneManager;
class Renderer;
class Window;
class InputManager;

class Scene {
public:
    Scene(Game* app, StringView name);
    virtual ~Scene() = default;

    virtual void OnEnter() {}

    virtual void OnLeave() {}

    virtual void OnUpdate(f32 dt) {}

    virtual void OnRender(f32 dt) {}

#if !defined(PD_NO_IMGUI)
    virtual void OnImGui() {}
#endif

    virtual void OnEvent(Event* event) {}

    StringView GetName();

    Camera& GetCamera();

protected:
    Game* app = nullptr;
    Ref<Renderer> renderer;
    ResourceCatalog& resources;
    Window& window;
    InputManager& input;
    SceneManager& sceneManager;

    String name;
    Camera cam;
};

}
