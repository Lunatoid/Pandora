#pragma once

#include "Pandora/Core/Data/Array.h"
#include "Pandora/Core/Event.h"

#include "Scene.h"

class Game;

namespace pd {

class SceneManager {
public:
    struct SceneEntry {
        u64 nameHash;
        Scene* scene;
    };

    SceneManager(Game* app);
    ~SceneManager();

    void Delete();

    void Update(f32 dt);

    void Render(f32 dt);

#if !defined(PD_NO_IMGUI)    
    void ImGui();
#endif

    void OnEvent(Event* event);

    template<typename T, typename ...Args>
    void AddScene(StringView name, Args&&... args) {
        static_assert(std::is_base_of<Scene, T>::value, "Template type T must derive from pd::Scene");

        u64 hash = DoHash(name);
        SceneEntry* scene = FindScene(hash);

        if (scene) {
            RemoveScene(name);
        }

        SceneEntry newScene = {};
        newScene.nameHash = hash;
        newScene.scene = New<T>(app, name, std::forward<Args>(args)...);

        scenes.Add(newScene);
    }

    void RemoveScene(StringView name);

    void SetCurrent(StringView name);

    Slice<SceneEntry> GetScenes();

    Scene* GetCurrent();

    Scene* GetScene(StringView name);

private:

    SceneEntry* FindScene(u64 hash);

    int FindSceneIndex(u64 hash);

    Game* app = nullptr;
    Scene* current = nullptr;
    Array<SceneEntry> scenes;
};

}
