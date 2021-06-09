#include "SceneManager.h"

#include "Pandora/Core/IO/Console.h"
#include "../Game.h"

namespace pd {

SceneManager::SceneManager(Game* app) : app(app) {}

SceneManager::~SceneManager() {
    Delete();
}

void SceneManager::Delete() {
    for (int i = 0; i < scenes.Count(); i++) {
        pd::Delete(scenes[i].scene);
    }
    scenes.Clear();
}

void SceneManager::Update(f32 dt) {
    if (current) {
        current->OnUpdate(dt);
    }
}

void SceneManager::Render(f32 dt) {
    if (current) {
        current->OnRender(dt);
    }
}

#if !defined(PD_NO_IMGUI)
void SceneManager::ImGui() {
    if (current) {
        current->OnImGui();
    }
}
#endif

void SceneManager::OnEvent(Event* event) {
    if (current) {
        current->OnEvent(event);
    }
}

void SceneManager::RemoveScene(StringView name) {
    u64 hash = DoHash(name);
    int index = FindSceneIndex(hash);

    if (index != -1) {
        pd::Delete(scenes[index].scene);
        scenes.Remove(index);
    } else {
        CONSOLE_LOG_DEBUG("[{}SceneManager Error{}] Scene {#} does not exist.\n", ConColor::Red, ConColor::White, name);
    }
}

void SceneManager::SetCurrent(StringView name) {
    u64 hash = DoHash(name);

    SceneEntry* scene = FindScene(hash);

    if (current) {
        current->OnLeave();
    }

    if (scene) {
        current = scene->scene;
        current->OnEnter();
    } else {
        CONSOLE_LOG_DEBUG("[{}SceneManager Error{}] Scene {#} does not exist.\n", ConColor::Red, ConColor::White, name);
    }
}

Slice<SceneManager::SceneEntry> SceneManager::GetScenes() {
    return scenes.Slice();
}

Scene* SceneManager::GetCurrent() {
    return current;
}

Scene* SceneManager::GetScene(StringView name) {
    SceneEntry* entry = FindScene(DoHash(name));

    return (entry) ? entry->scene : nullptr;
}

SceneManager::SceneEntry* SceneManager::FindScene(u64 hash) {
    int index = FindSceneIndex(hash);
    return (index != -1) ? &scenes[index] : nullptr;
}

int SceneManager::FindSceneIndex(u64 hash) {
    return scenes.Find([&](SceneEntry& entry) {
        return entry.nameHash == hash;
    }).ValueOr(-1);
}

}
