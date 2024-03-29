#pragma once

#include "Pandora/Core.h"
#include "Pandora/Graphics.h"
#include "Pandora/Audio.h"

#include "Scene/Scene.h"
#include "Scene/SceneManager.h"

using namespace pd;

class GameMasterScene : public Scene {
public:
    GameMasterScene(Game* app, StringView name, int boardCount, JsonValue* config);

    virtual void OnEnter() override;
    virtual void OnLeave() override;

    virtual void OnUpdate(f32 dt) override;
    virtual void OnRender(f32 dt) override;

private:
    SceneManager boardManager;
    int currentBoard = 0;
    int boardCount = 0;

    f32 minSwitchTime = 3.0f;
    f32 maxSwitchTime = 15.0f;

    f32 nextSwitchTime = 0.0f;

    Sound bgm;
};
