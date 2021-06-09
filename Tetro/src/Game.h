#pragma once

#include "MenuScene.h"
#include "PauseScene.h"

#include "Pandora/App.h"

using namespace pd;

class Game : public App {
public:
    Game(int argc, char** argv, VideoBackend backend, JsonValue* config) :
        App(argc, argv, backend, "Tetro", Vec2i(580, 720)), config(config), sceneManager(this) {
        catalog.Load("Tetro.box");

        InitStorage("TomMol", "Tetro");

        FileStream stream;
        if (CreateStorageFile("ThirdPartyLicenses.txt", stream)) {
            Ref<BinaryResource> licenses = catalog.Get<BinaryResource>("Licenses");
            stream.WriteBytes(licenses->GetData());
            stream.Close();
        }

        sceneManager.AddScene<MenuScene>("Menu", config);
        sceneManager.AddScene<PauseScene>("Pause");
        sceneManager.SetCurrent("Menu");
    }

    ~Game() {
        Delete(config);
    }

    virtual void OnUpdate(f32 dt) override {
        sceneManager.Update(dt);
    }

    virtual void OnRender(f32 dt) override {
        sceneManager.Render(dt);
    }

    virtual void OnImGui() override {
        sceneManager.ImGui();
    }

    SceneManager& GetSceneManager() {
        return sceneManager;
    }

private:
    JsonValue* config = nullptr;
    SceneManager sceneManager;
};