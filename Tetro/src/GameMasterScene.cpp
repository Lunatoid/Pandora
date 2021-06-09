#include "GameMasterScene.h"

#include "GameScene.h"

GameMasterScene::GameMasterScene(Game* app, StringView name, int boardCount, JsonValue* config) :
    Scene(app, name), boardCount(boardCount), boardManager(app) {

    Optional<f64> bgmVolume = config->TryGetNumber("bgm_volume");
    if (bgmVolume) {
        bgm.volume = (f32)bgmVolume.Value();
    }

    Optional<f64> configMinSwitchTime = config->TryGetNumber("min_switch_time");
    if (configMinSwitchTime) {
        minSwitchTime = (f32)configMinSwitchTime.Value();
    }

    Optional<f64> configMaxSwitchTime = config->TryGetNumber("max_switch_time");
    if (configMaxSwitchTime) {
        maxSwitchTime = (f32)configMaxSwitchTime.Value();
    }

    BoundedString<8> boardName;
    for (int i = 0; i < boardCount; i++) {
        boardName.Format("{}", i);
        boardManager.AddScene<GameScene>(boardName, config);
    }

    if (boardCount > 1) {
        currentBoard = pd::random.Range(0, boardCount);

        boardName.Format("{}", currentBoard);
        boardManager.SetCurrent(boardName);

        nextSwitchTime = (f32)pd::random.Range(minSwitchTime, maxSwitchTime);
    } else {
        boardManager.SetCurrent("0");
    }

    bgm.Load("GameBGM");
    bgm.SetLooping(true);
    bgm.Play();
}

void GameMasterScene::OnEnter() {
    bgm.Resume();
}

void GameMasterScene::OnLeave() {
    bgm.Pause();
}

void GameMasterScene::OnUpdate(f32 dt) {
    if (boardCount > 1) {
        nextSwitchTime -= dt;

        if (nextSwitchTime < 0.0f) {
            // Time to switch!
            int newBoard = 0;
            do {
                newBoard = pd::random.Range(0, boardCount);
            } while (newBoard == currentBoard);

            currentBoard = newBoard;

            BoundedString<8> boardName;
            boardName.Format("{}", currentBoard);
            boardManager.SetCurrent(boardName);

            nextSwitchTime = (f32)pd::random.Range(minSwitchTime, maxSwitchTime);
        }
    }

    if (input.IsKeyPressed(Key::Escape)) {
        sceneManager.SetCurrent("Pause");
    }

    boardManager.Update(dt);
}

void GameMasterScene::OnRender(f32 dt) {
    boardManager.Render(dt);
}
