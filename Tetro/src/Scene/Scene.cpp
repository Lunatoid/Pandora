#include "Scene.h"

#include "SceneManager.h"
#include "../Game.h"

namespace pd {

Scene::Scene(Game* app, StringView name) : app(app), resources(ResourceCatalog::Get()),
    window(app->GetWindow()), input(app->GetInputManager()), sceneManager(app->GetSceneManager()) {
    renderer = app->GetRenderer();
    this->name.Set(name);
}

StringView Scene::GetName() {
    return name;
}

Camera& Scene::GetCamera() {
    return cam;
}

}
