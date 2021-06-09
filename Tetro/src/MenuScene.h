#pragma once

#include "Pandora/Core.h"
#include "Pandora/Graphics.h"
#include "Pandora/Audio.h"

#include "Scene/Scene.h"

using namespace pd;

class MenuScene : public Scene {
public:
    MenuScene(Game* app, StringView name, JsonValue* config);

    virtual void OnEnter() override;
    virtual void OnLeave() override;

    virtual void OnUpdate(f32 dt) override;
    virtual void OnRender(f32 dt) override;

private:
    SpriteRenderer spriteRenderer;
    Vec2 optionOffset;

    Array<Text> options;
    int index = 0;

    Sound bgm;
    Sound selectSfx;
    Sound confirmSfx;

    Sprite bg;
    JsonValue* config = nullptr;
};
