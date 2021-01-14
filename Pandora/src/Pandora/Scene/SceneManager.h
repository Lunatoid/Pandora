#pragma once

#include "Pandora/Core/Data/Array.h"
#include "Pandora/Core/Event.h"

#include "Pandora/Scene/Scene.h"

namespace pd {

class App;

class SceneManager {
public:
    struct SceneEntry {
        u64 nameHash;
        Scene* scene;
    };

    SceneManager(App* app);
    ~SceneManager();
    
    /// <summary>
    /// Deletes all memory associated with the scene manager.
    /// </summary>
    void Delete();
    
    /// <summary>
    /// Updates the current scene.
    /// </summary>
    /// <param name="dt">The update delta time.</param>
    void Update(f32 dt);
    
    /// <summary>
    /// Renders the current scene/
    /// </summary>
    /// <param name="dt">The render delta time.</param>
    void Render(f32 dt);

#if !defined(PD_NO_IMGUI)    
    /// <summary>
    /// Calls ImGui for the current scene.
    /// </summary>
    void ImGui();
#endif

    /// <summary>
    /// Pumps events for the current scene.
    /// </summary>
    /// <param name="event">The event.</param>
    void OnEvent(Event* event);

    /// <summary>
    /// Adds a new scene.
    /// </summary>
    /// <param name="name">The name. Must be unique.</param>
    /// <param name="args">The contructor args.</param>
    template<typename T, typename ...Args>
    void AddScene(StringView name, Args&&... args) {
        static_assert(std::is_base_of<Scene, T>::value, "Template type T must derive from pd::Scene");

        u64 hash = DoHash(&name);
        SceneEntry* scene = FindScene(hash);

        if (scene) {
            RemoveScene(name);
        }

        SceneEntry newScene = {};
        newScene.nameHash = hash;
        newScene.scene = New<T>(app, name, std::forward<Args>(args)...);

        scenes.Add(newScene);
    }
    
    /// <summary>
    /// Removes the specified scene.
    /// </summary>
    /// <param name="name">The name.</param>
    void RemoveScene(StringView name);
    
    /// <summary>
    /// Switches to the specified scene.
    /// </summary>
    /// <param name="name">The name.</param>
    void SetCurrent(StringView name);
    
    /// <summary>
    /// Returns a slice with all the scenes.
    /// </summary>
    /// <returns>A slice with all the scenes.</returns>
    Slice<SceneEntry> GetScenes();
    
    /// <summary>
    /// Returns the current scene.
    /// </summary>
    /// <returns>The current scene.</returns>
    Scene* GetCurrent();

    /// <summary>
    /// Returns a scene by name.
    /// </summary>
    /// <returns>The scene. Returns <c>nullptr</c> if not found.</returns>
    Scene* GetScene(StringView name);

private:    
    /// <summary>
    /// Finds the scene by the hash.
    /// </summary>
    /// <param name="hash">The hash.</param>
    /// <returns>The scene.</returns>
    SceneEntry* FindScene(u64 hash);
    
    /// <summary>
    /// Finds the index of the scene by the hash.
    /// </summary>
    /// <param name="hash">The hash.</param>
    /// <returns>The index.</returns>
    int FindSceneIndex(u64 hash);

    App* app = nullptr;
    Scene* current = nullptr;
    Array<SceneEntry> scenes;
};

}
