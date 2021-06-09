#pragma once

#include "Pandora/Core.h"
#include "Pandora/Graphics.h"

#include "SpriteAtlas.h"

using namespace pd;

// @HARDCODED
const Vec2 TILE_SIZE = Vec2(32.0f);

enum class TileType : byte {
    Empty,
    Cyan,   // I
    Yellow, // O
    Purple, // T
    Green,  // S
    Red,    // Z
    Blue,   // J
    Orange, // L
    Count
};

const StringView TILE_TYPE_NAMES[] = {
    "Empty",
    "Cyan",
    "Yellow",
    "Purple",
    "Green",
    "Red",
    "Blue",
    "Orange"
};

// Count minus the empty tile
const int TETROPIECE_COUNT = (int)TileType::Count - 1;

typedef bool TetroShape[4 * 4];

enum class Rotation {
    Degrees0,
    Degrees270,
    Degrees180,
    Degrees90,
    Count
};

/**
 * \brief Gets the index of the character when rotated by the rotation.
 * 
 * \param x The x position.
 * \param y The y position.
 * \param rotation The rotation.
 * \return The rotated index.
 */
int GetRotatedIndex(int x, int y, Rotation rotation);

class TetroPiece {
public:

    /**
     * \brief Sets the shape. Shape must be 4x4 where 'X' represents a solid 
     * space and '.' represents air.
     * 
     * \param shape The shape string.
     */
    void SetShape(StringView shape);

    /**
     * \brief Sets the tile type.
     * 
     * \param type The type.
     */
    void SetType(TileType type);

    /**
     * \brief Sets the rotational offset for each rotation.
     * 
     * \param degrees0 The 0 degrees offset.
     * \param degrees90 The 90 degrees offset.
     * \param degrees180 The 180 degrees offset.
     * \param degrees270 The 270 degrees offset.
     */
    void SetRotationOffset(Vec2i degrees0, Vec2i degrees90, Vec2i degrees180, Vec2i degrees270);

    /**
     * \return The tile type.
     */
    TileType GetType() const;

    /**
     * \brief Gets the shape.
     * 
     * \param shape The output shape.
     */
    void GetShape(TetroShape shape);

    /**
     * \param rotation The rotation.
     * \return The offset.
     */
    Vec2i GetRotationOffset(Rotation rotation);

    /**
     * \brief Gets the piece in the specified rotation.
     * 
     * \param shape The output hsape.
     * \param rotation The rotation.
     */
    void GetRotatedShape(TetroShape shape, Rotation rotation);

    /**
     * \brief Draws the piece.
     * 
     * \param spriteRenderer The renderer to use.
     * \param position The position.
     * \param rotation The rotation.
     * \param atlas The tileatlas.
     * \param alpha The alpha.
     */
    void Draw(SpriteRenderer& spriteRenderer, Vec3 position, Rotation rotation,
              Ref<SpriteAtlas> atlas, f32 alpha);

private:
    TileType type = TileType::Empty;
    TetroShape shape = {};
    Vec2i rotationOffset[(int)Rotation::Count];
};
