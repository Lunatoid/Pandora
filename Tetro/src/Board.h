#pragma once

#include <Pandora/Audio.h>

#include "TetroPiece.h"

// @HARDCODED
const int BOARD_WIDTH = 10;
const int BOARD_HEIGHT = 40;
const int BOARD_VISIBLE_HEIGHT = 21;

class Board {
public:
    Board(StringView tileAtlasName);
    
    /// <summary>
    /// Checks if the Tetro piece fits on the board.
    /// </summary>
    /// <param name="position">The position.</param>
    /// <param name="piece">The piece.</param>
    /// <param name="rotation">The rotation.</param>
    /// <returns>Whether or not it fits.</returns>
    bool DoesPieceFit(Vec2i position, TetroPiece* piece, Rotation rotation);
    
    /// <summary>
    /// Adds the tetro piece to the board.
    /// </summary>
    /// <param name="position">The position.</param>
    /// <param name="piece">The piece.</param>
    /// <param name="rotation">The rotation.</param>
    void AddPieceToBoard(Vec2i position, TetroPiece* piece, Rotation rotation);
    
    /// <summary>
    /// Draws the board with the specified renderer.
    /// </summary>
    /// <param name="spriteRenderer">The sprite renderer.</param>
    void Draw(SpriteRenderer& spriteRenderer);
    
    /// <summary>
    /// Resets the score and clears the board.
    /// </summary>
    void Clear();
    
    /// <summary>
    /// Gets the tile at the position.
    /// </summary>
    /// <param name="x">The x position.</param>
    /// <param name="y">The y position.</param>
    /// <returns>The tile.</returns>
    TileType GetTileAtPosition(int x, int y);
    
    /// <summary>
    /// Stops all the SFX from playing.
    /// </summary>
    void StopSFX();
    
    /// <summary>
    /// Sets custom clear scores.
    /// </summary>
    /// <param name="scores">The scores.</param>
    void SetClearScores(int scores[4]);
    
    /// <summary>
    /// Sets the volume for all the SFXs.
    /// </summary>
    /// <param name="volume">The volume.</param>
    void SetSFXVolume(f32 volume);
    
    /// <summary>
    /// Gets the current score.
    /// </summary>
    /// <returns>The current score.</returns>
    int GetScore();
    
    /// <summary>
    /// Gets a reference to the tile atlas.
    /// </summary>
    /// <param name="type">The reference type.</param>
    /// <returns>The atlas.</returns>
    Ref<SpriteAtlas> GetTileAtlas(RefType type = RefType::Strong);

    Vec3 position;
    Vec2 scale = Vec2(1.0f);

private:
    // Playfield includes the so-called "vanish zone"
    TileType board[BOARD_HEIGHT][BOARD_WIDTH] = {};

    Ref<SpriteAtlas> tileAtlas;

    Sound clearSfx;
    Sound superClearSfx;

    int clearScores[4] = {
        40,
        100,
        300,
        800
    };

    // @TODO: implement level mechanic
    int level = 0;
    int score = 0;
};
