#include "GameScene.h"

Rotation NextRotation(Rotation current, bool clockwise = true) {
    if (clockwise) {
        return (Rotation)((int(current) + 1) % (int)Rotation::Count);
    } else {
        int next = (int)current - 1;
        if (next < 0) {
            next = (int)Rotation::Count - 1;
        }
        return Rotation(next);
    }
}

GameScene::GameScene(Game* app, StringView name, JsonValue* config) : Scene(app, name), board("Tiles") {
    // Nudge camera
    Vec2i size = window.GetSize();

    cam.Init(size);
    cam.position.xy += (Vec2)size / 2.0f;

    board.position = Vec3(TILE_SIZE.x, TILE_SIZE.y, 100.0f);
    tileAtlas = board.GetTileAtlas();
    bg.Load("GameBG");

    // Load config
    tweenY = config->TryGetBool("smooth_fall").ValueOr(tweenY);
    sonicLock = config->TryGetBool("sonic_lock").ValueOr(sonicLock);

    Optional<JsonValue> scores = config->TryGetArray("scores");
    if (scores) {
        // @HARDCODED: default scores
        int clearScores[4] = {
            40,
            100,
            300,
            800
        };

        clearScores[0] = scores->TryGetNumber(0).ValueOr(clearScores[0]);
        clearScores[1] = scores->TryGetNumber(1).ValueOr(clearScores[1]);
        clearScores[2] = scores->TryGetNumber(2).ValueOr(clearScores[2]);
        clearScores[3] = scores->TryGetNumber(3).ValueOr(clearScores[3]);

        board.SetClearScores(clearScores);
    }

    startingFallRate = config->TryGetNumber("starting_fall_speed").ValueOr(startingFallRate);
    fallRateInSeconds = startingFallRate;

    finalFallRate = config->TryGetNumber("final_fall_speed").ValueOr(finalFallRate);

    maxScore = (int)config->TryGetNumber("max_score").ValueOr(maxScore);

    Optional<f64> sfxVolume = config->TryGetNumber("sfx_volume");
    if (sfxVolume) {
        dropSfx.volume = (f32)sfxVolume.Value();
        rotateSfx.volume = (f32)sfxVolume.Value();
        holdSfx.volume = (f32)sfxVolume.Value();

        board.SetSFXVolume((f32)sfxVolume.Value());
    }

    Optional<JsonValue> piecesConfig = config->TryGetArray("pieces");
    if (piecesConfig) {
        int count = piecesConfig->Count();
        for (int i = 0; i < count; i++) {
            Optional<JsonValue> pieceConfig = piecesConfig->TryGetObject(i);
            if (piecesConfig) {
                String shape;
                shape = pieceConfig->TryGetString("shape").ValueOr("....\nX.X.\n.X..\nX.X.");

                Optional<String> colorStr = pieceConfig->TryGetString("color");
                TileType color = TileType::Cyan;
                if (colorStr) {
                    // Start at 1 to exclude TileType::Empty
                    for (int i = 1; i < (int)TileType::Count; i++) {
                        if (colorStr.Value() == TILE_TYPE_NAMES[i]) {
                            color = (TileType)i;
                            break;
                        }
                    }
                }

                Vec2i rotations[4] = {
                    Vec2i(0, -1),
                    Vec2i(1, 0),
                    Vec2i(0, 1),
                    Vec2i(-1, 0)
                };

                Optional<JsonValue> offsetConfig = pieceConfig->TryGetArray("offset");
                if (offsetConfig) {
                    for (int i = 0; i < 4; i++) {
                        Optional<JsonValue> offsetArray = offsetConfig->TryGetArray(i);
                        rotations[i].x = offsetArray->TryGetNumber(0).ValueOr(rotations[i].x);
                        rotations[i].y = offsetArray->TryGetNumber(1).ValueOr(rotations[i].y);
                    }
                }

                pieces.Add();
                pieces.Last().SetType(color);
                pieces.Last().SetShape(shape);
                pieces.Last().SetRotationOffset(rotations[0], rotations[1], rotations[2], rotations[3]);
            }
        }
    } else {
        // Default pieces
        pieces.Add();
        pieces.Last().SetType(TileType::Cyan);
        pieces.Last().SetShape(R"(.... 
                              ....
                              XXXX
                              ....)");
        pieces.Last().SetRotationOffset(Vec2i(0, -1), Vec2i(1, 0), Vec2i(0, 1), Vec2i(-1, 0));

        pieces.Add();
        pieces.Last().SetType(TileType::Red);
        pieces.Last().SetShape("....\n....\n.XX.\nXX..");
        pieces.Last().SetRotationOffset(Vec2i(0, -1), Vec2i(1, 0), Vec2i(0, 1), Vec2i(-1, 0));

        pieces.Add();
        pieces.Last().SetType(TileType::Green);
        pieces.Last().SetShape("....\n....\nXX..\n.XX.");
        pieces.Last().SetRotationOffset(Vec2i(0, -1), Vec2i(1, 0), Vec2i(0, 1), Vec2i(-1, 0));

        pieces.Add();
        pieces.Last().SetType(TileType::Yellow);
        pieces.Last().SetShape("....\n....\nXX..\nXX..");
        pieces.Last().SetRotationOffset(Vec2i(0, -2), Vec2i(2, 0), Vec2i(0, 2), Vec2i(-2, 0));

        pieces.Add();
        pieces.Last().SetType(TileType::Purple);
        pieces.Last().SetShape("....\n....\nXXX.\n.X..");
        pieces.Last().SetRotationOffset(Vec2i(0, -1), Vec2i(1, 0), Vec2i(0, 1), Vec2i(-1, 0));

        pieces.Add();
        pieces.Last().SetShape("....\n....\nXXX.\n..X.");
        pieces.Last().SetType(TileType::Orange);
        pieces.Last().SetRotationOffset(Vec2i(0, -1), Vec2i(1, 0), Vec2i(0, 1), Vec2i(-1, 0));

        pieces.Add();
        pieces.Last().SetShape("....\n....\nXXX.\nX...");
        pieces.Last().SetType(TileType::Blue);
        pieces.Last().SetRotationOffset(Vec2i(0, -1), Vec2i(1, 0), Vec2i(0, 1), Vec2i(-1, 0));
    }

    bag.Init(pieces.Count());
    bag.Shake();

    NewGhost();

    // Initialize HUD
    heldText.Load("GuiFont");
    heldText.text.Set("Held:");
    heldText.scale = Vec2(0.33f);
    heldText.position = Vec3(BOARD_WIDTH + 2.0f, BOARD_VISIBLE_HEIGHT - 1, 100.0f) * Vec3(TILE_SIZE.x, TILE_SIZE.y, 200.0f);
    heldText.GenerateSprites();

    nextText.Load("GuiFont");
    nextText.text.Set("Next:");
    nextText.scale = Vec2(0.33f);
    nextText.position = Vec3(BOARD_WIDTH + 2.0f, BOARD_VISIBLE_HEIGHT - 6, 100.0f) * Vec3(TILE_SIZE.x, TILE_SIZE.y, 200.0f);
    nextText.GenerateSprites();

    scoreText.Load("GuiFont");
    scoreText.text.Set("Score: 0");
    scoreText.scale = Vec2(0.33f);
    scoreText.position = Vec3(4.0f, -TILE_SIZE.y / 2.0f, 200.0f);
    scoreText.GenerateSprites();

    dropSfx.Load("SFX/Drop");
    rotateSfx.Load("SFX/Rotate");
    holdSfx.Load("SFX/Hold");
}

void GameScene::OnLeave() {
    dropSfx.Stop();
    rotateSfx.Stop();
    holdSfx.Stop();

    board.StopSFX();
}

void GameScene::OnUpdate(f32 dt) {
    fallTimer += dt;

    // Lock to the ground
    bool canGoDown = board.DoesPieceFit(ghostPos - Vec2i(0, 1), ghostPiece, ghostRotation);

    subY = (canGoDown && tweenY) ? fallTimer / fallRateInSeconds : 0.0f;
    if (fallTimer >= fallRateInSeconds && canGoDown) {
        if (canGoDown) {
            fallTimer = 0.0f;
            ghostPos.y -= 1;
            lockDelayTimer = 0.0f;
            lockToGround = false;
            subY = 0.0f;

            // Recalculate
            canGoDown = board.DoesPieceFit(ghostPos - Vec2i(0, 1), ghostPiece, ghostRotation);
        }
    }

    if (!canGoDown) {
        lockToGround = true;
    }

    // Check if we can lock
    if (lockToGround) {
        if (lockDelayTimer >= lockDelayInSeconds) {
            lockToGround = false;
            lockDelayTimer = 0.0f;
            alreadyHeld = false;

            dropSfx.Play();

            board.AddPieceToBoard(ghostPos, ghostPiece, ghostRotation);
            NewGhost();

            // Update score
            scoreText.text.Format("Score: {}", board.GetScore());
            scoreText.GenerateSprites();

            // Update speed
            fallRateInSeconds = Lerp(startingFallRate, finalFallRate, board.GetScore() / (f32)maxScore);

        } else {
            lockDelayTimer += dt;
        }
    }

    // Rotate
    if (input.IsKeyPressed(Key::Z) || input.IsKeyPressed(Key::X)) {
        bool rotationSuccess = true;
        Vec2i wallKickOffset = Vec2i(0);

        bool clockwise = input.IsKeyPressed(Key::Z);

        Rotation rotationForOffset = (clockwise) ? NextRotation(ghostRotation) : ghostRotation;
        Vec2i baseOffset = ghostPiece->GetRotationOffset(rotationForOffset);

        if (!clockwise) {
            baseOffset = -baseOffset;
        }

        Vec2i offsetPos = ghostPos + baseOffset;

        Rotation nextRotation = NextRotation(ghostRotation, clockwise);
        if (!board.DoesPieceFit(offsetPos, ghostPiece, nextRotation)) {
            // Tetromino doesn't fit, we must wall kick
            // Will check from offsetPos.x -..+ WALL_KICK_RANGE
            const int WALL_KICK_RANGE = 2;

            rotationSuccess = false;
            for (int x = 1; x <= WALL_KICK_RANGE; x++) {
                if (board.DoesPieceFit(offsetPos + Vec2i(x, 0), ghostPiece, nextRotation)) {
                    rotationSuccess = true;
                    wallKickOffset.x = x;
                    break;
                }
            }

            if (!rotationSuccess) {
                for (int x = -1; x >= -WALL_KICK_RANGE; x--) {
                    if (board.DoesPieceFit(offsetPos + Vec2i(x, 0), ghostPiece, nextRotation)) {
                        rotationSuccess = true;
                        wallKickOffset.x = x;
                        break;
                    }
                }
            }
        }

        if (rotationSuccess) {
            rotateSfx.Play();

            ghostRotation = nextRotation;
            ghostPos = offsetPos + wallKickOffset;

            // We reset the lock delay on rotation in this household
            lockDelayTimer = 0.0f;
        }
    }

    // Hold
    if (!alreadyHeld && input.IsKeyPressed(Key::C)) {
        holdSfx.Play();

        TetroPiece* currentHeld = heldPiece;
        heldPiece = ghostPiece;
        NewGhost(currentHeld);
        alreadyHeld = true;
    }

    // Move left
    if (input.IsKeyRepeated(Key::Left) &&
        board.DoesPieceFit(ghostPos - Vec2i(1, 0), ghostPiece, ghostRotation)) {
        ghostPos.x -= 1;
        lockDelayTimer = 0.0f;
    }

    // Move right
    if (input.IsKeyRepeated(Key::Right) &&
        board.DoesPieceFit(ghostPos + Vec2i(1, 0), ghostPiece, ghostRotation)) {
        ghostPos.x += 1;
        lockDelayTimer = 0.0f;
    }

    // Move down
    if (input.IsKeyRepeated(Key::Down) &&
        board.DoesPieceFit(ghostPos - Vec2i(0, 1), ghostPiece, ghostRotation)) {
        ghostPos.y -= 1;
    }

    // Hard drop
    if (input.IsKeyPressed(Key::Space)) {
        int y = 0;
        while (board.DoesPieceFit(ghostPos - Vec2i(0, ++y), ghostPiece, ghostRotation));
        ghostPos.y -= y - 1;

        if (sonicLock) {
            lockToGround = true;
            lockDelayTimer = lockDelayInSeconds;
        }
    }

    // Reset
    if (input.IsKeyPressed(Key::R)) {
        ResetBoard();
    }

    cam.UpdateProjection();
}

void GameScene::OnRender(f32 dt) {
    spriteRenderer.UpdateProjection(cam);
    board.Draw(spriteRenderer);
    spriteRenderer.Draw(bg);

    Vec3 worldPos = board.position + Vec3(ghostPos.x * TILE_SIZE.x, ghostPos.y * TILE_SIZE.y, 1.0f);
    ghostPiece->Draw(spriteRenderer, worldPos - Vec3(0.0f, subY * TILE_SIZE.y, 0.0f), ghostRotation, tileAtlas, 1.0f);

    // Draw preview
    int lowestY = 1;
    while (board.DoesPieceFit(ghostPos - Vec2i(0, lowestY), ghostPiece, ghostRotation)) {
        ++lowestY;
    }
    --lowestY;

    ghostPiece->Draw(spriteRenderer, worldPos - Vec3(0.0f, lowestY * TILE_SIZE.y, 0.0f), ghostRotation, tileAtlas, 0.33f);

    // Draw held
    Vec3 boardEnd = board.position + Vec3(BOARD_WIDTH * TILE_SIZE.x, BOARD_VISIBLE_HEIGHT * TILE_SIZE.y, 0.0f);
    if (heldPiece) {
        heldPiece->Draw(spriteRenderer, boardEnd + Vec3(TILE_SIZE.x * 2.0f, -TILE_SIZE.y * 6.0f, 0.0f), Rotation::Degrees0, tileAtlas, 1.0f);
    }

    // Draw next
    bag.DrawNext(spriteRenderer, boardEnd + Vec3(TILE_SIZE.x * 2.0f, -TILE_SIZE.y * 11.0f, 100.0f), pieces, tileAtlas, 5);

    // Draw HUD
    heldText.Draw(spriteRenderer);
    nextText.Draw(spriteRenderer);
    scoreText.Draw(spriteRenderer);

    spriteRenderer.Render();
}

void GameScene::ResetBoard() {
    fallRateInSeconds = startingFallRate;
    fallTimer = 0.0f;
    lockDelayTimer = 0.0f;

    board.Clear();

    bag.Shake();
    heldPiece = nullptr;
    NewGhost();

    scoreText.text.Set("Score: 0");
    scoreText.GenerateSprites();
}

void GameScene::NewGhost(TetroPiece* preferred) {
    // Generate new tetromino
    TetroPiece* newPiece = (preferred) ? preferred : &pieces[bag.GetNextIndex()];

    ghostRotation = Rotation::Degrees0;
    ghostPiece = newPiece;
    ghostPos = Vec2i(BOARD_WIDTH / 2 - 2, BOARD_VISIBLE_HEIGHT - 1);

    if (!board.DoesPieceFit(ghostPos, ghostPiece, ghostRotation)) {
        // GAME OVER
        ResetBoard();
    }
}
