#include "MenuScene.h"

#include "Game.h"

#include "GameMasterScene.h"

MenuScene::MenuScene(Game* app, StringView name, JsonValue* config) : Scene(app, name), config(config) {

    auto addOption = [&](StringView name) {
        options.Add();
        options.Last().text.Set(name);
        options.Last().scale = Vec2(0.5f);
        options.Last().Load("GuiFont");
        options.Last().GenerateSprites();
    };

    addOption("Normal");
    addOption("Double Trouble");
    addOption("Inferno");
    addOption("\nQuit");

    Optional<f64> sfxVolume = config->TryGetNumber("sfx_volume");
    if (sfxVolume) {
        selectSfx.volume = (f32)sfxVolume.Value();
        confirmSfx.volume = (f32)sfxVolume.Value();
    }

    Optional<f64> bgmVolume = config->TryGetNumber("bgm_volume");
    if (bgmVolume) {
        bgm.volume = (f32)bgmVolume.Value();
    }

    selectSfx.Load("SFX/Drop");
    confirmSfx.Load("SFX/Clear");

    bg.Load("MenuBG");

    bgm.Load("MenuBGM");
    bgm.SetLooping(true);

    Vec2i size = window.GetSize();
    optionOffset.x = size.x / 9.0f;
    optionOffset.y = size.y / 2.0f;
    cam.Init(size);
    cam.position.xy += (Vec2)size / 2.0f;
}

void MenuScene::OnEnter() {
    index = 0;

    bgm.Play();
}

void MenuScene::OnLeave() {
    bgm.Stop();
}

void MenuScene::OnUpdate(f32 dt) {
    if (input.IsKeyRepeated(Key::Up)) {
        selectSfx.Play();

        if (index == 0) {
            index = options.Count() - 1;
        } else {
            index -= 1;
        }
    }

    if (input.IsKeyRepeated(Key::Down)) {
        selectSfx.Play();

        if (index == options.Count() - 1) {
            index = 0;
        } else {
            index += 1;
        }
    }

    if (input.IsKeyRepeated(Key::Space)) {
        confirmSfx.Play();

        // Ew
        switch (index) {
            case 0:
                sceneManager.AddScene<GameMasterScene>("Game", 1, config);
                sceneManager.SetCurrent("Game");
                break;

            case 1:
                sceneManager.AddScene<GameMasterScene>("Game", 2, config);
                sceneManager.SetCurrent("Game");
                break;

            case 2:
                sceneManager.AddScene<GameMasterScene>("Game", 5, config);
                sceneManager.SetCurrent("Game");
                break;

            case 3:
                app->Quit();
                break;
        }
    }

    f32 nextOffset = 0.0f;
    for (int i = 0; i < options.Count(); i++) {
        options[i].position = Vec2(optionOffset - Vec2(0.0f, nextOffset));
        options[i].color = (i == index) ? Color(1.0f, 1.0f, 0.0f, 1.0f) : Color(1.0f);
        options[i].UpdateProperties();
        
        nextOffset += options[i].CalculateBounds().y * 1.1f;
    }

    cam.UpdateProjection();
}

void MenuScene::OnRender(f32 dt) {
    spriteRenderer.UpdateProjection(cam);

    for (int i = 0; i < options.Count(); i++) {
        options[i].Draw(spriteRenderer);
    }
    spriteRenderer.Draw(bg);
    spriteRenderer.Render();
}
