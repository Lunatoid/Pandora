#pragma once

#include "Pandora/Core.h"
#include "Pandora/Graphics.h"

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

int GetRotatedIndex(int x, int y, Rotation rotation);

class TetroPiece {
public:    
    /// <summary>
    /// Sets the shape. Shape must be 4x4 where 'X' represents filled space and '.' represents empty space.
    /// Any other characters are ignored.
    /// </summary>
    /// <param name="shape">The shape.</param>
    void SetShape(StringView shape);
    
    /// <summary>
    /// Sets the tile type.
    /// </summary>
    /// <param name="type">The type.</param>
    void SetType(TileType type);
    
    /// <summary>
    /// Sets the position offset for each rotation.
    /// </summary>
    /// <param name="degrees0">The offset for 0 degrees.</param>
    /// <param name="degrees90">The offset for 90 degrees.</param>
    /// <param name="degrees180">The offset for 180 degrees..</param>
    /// <param name="degrees270">The offset for 270 degrees.</param>
    void SetRotationOffset(Vec2i degrees0, Vec2i degrees90, Vec2i degrees180, Vec2i degrees270);
    
    /// <summary>
    /// Gets the tile type.
    /// </summary>
    /// <returns>The tile type.</returns>
    TileType GetType() const;
    
    /// <summary>
    /// Gets the shape.
    /// </summary>
    /// <param name="shape">The shape output.</param>
    void GetShape(TetroShape shape);
    
    /// <summary>
    /// Gets the rotation offset.
    /// </summary>
    /// <param name="rotation">The rotation.</param>
    /// <returns>The offset.</returns>
    Vec2i GetRotationOffset(Rotation rotation);
    
    /// <summary>
    /// Gets the rotated shape.
    /// </summary>
    /// <param name="shape">The shape output.</param>
    /// <param name="rotation">The rotation.</param>
    void GetRotatedShape(TetroShape shape, Rotation rotation);
    
    /// <summary>
    /// Draws the piece with the specified renderer.
    /// </summary>
    /// <param name="spriteRenderer">The sprite renderer.</param>
    /// <param name="position">The position.</param>
    /// <param name="rotation">The rotation.</param>
    /// <param name="atlas">The atlas.</param>
    /// <param name="alpha">The alpha.</param>
    void Draw(SpriteRenderer& spriteRenderer, Vec3 position, Rotation rotation, Ref<SpriteAtlas> atlas, f32 alpha);

private:
    TileType type = TileType::Empty;
    TetroShape shape = {};
    Vec2i rotationOffset[(int)Rotation::Count];
};
