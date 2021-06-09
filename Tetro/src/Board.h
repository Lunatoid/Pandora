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

    /**
     * \brief Checks if the Tetro piece fits on the board.
     * 
     * \param position The position.
     * \param piece The piece.
     * \param rotation The rotation.
     * \return Whether or not the piece fits.
     */
    bool DoesPieceFit(Vec2i position, TetroPiece* piece, Rotation rotation);

    /**
     * \brief Adds the piece to the board.
     * 
     * \param position The position.
     * \param piece The piece.
     * \param rotation The rotation.
     */
    void AddPieceToBoard(Vec2i position, TetroPiece* piece, Rotation rotation);

    /**
     * \brief Draws the board.
     * 
     * \param spriteRenderer The renderer to draw the board with.
     */
    void Draw(SpriteRenderer& spriteRenderer);

    /**
     * \brief Resets the score and clears the board.
     */
    void Clear();

    /**
     * \brief Gets the tile at the specified position.
     * 
     * \param x The x position.
     * \param y The y position
     * \return The tile.
     */
    TileType GetTileAtPosition(int x, int y);

    /**
     * \brief Stops all sound effects from playing.
     */
    void StopSFX();

    /**
     * \brief Sets the custom clear sores.
     * 
     * \param scores The scores.
     */
    void SetClearScores(int scores[4]);

    /**
     * \brief Sets the volume of all the sound effects.
     * 
     * \param volume The volume.
     */
    void SetSFXVolume(f32 volume);

    /**
     * \return The current score.
     */
    int GetScore();

    /**
     * \param type The reference type.
     * \return A new reference to the atlas.
     */
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
