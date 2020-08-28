#pragma once

#include "Pandora/Graphics/Sprite/Sprite.h"

namespace pd {

// @TODO: improve this class, it's a bit messy
class SpriteAtlas {
public:
    // 2D array intermediate
    struct SpriteAtlasRow {
        SpriteAtlasRow(int y, SpriteAtlas& parent) : y(y), parent(parent) {}

        int y = 0;
        SpriteAtlas& parent;

        Sprite* operator[](int x) {
            return parent.Get(x, y);
        }
    };
    
    /// <summary>
    /// Loads the base sprite.
    /// </summary>
    /// <param name="offset">The offset in pixels from the top left.</param>
    /// <param name="tileSize">Size of each tile.</param>
    /// <param name="texName">Name of the texture.</param>
    /// <param name="shaderName">Name of the shader.</param>
    void Load(Vec2 offset, Vec2 tileSize, StringView texName, StringView shaderName = "Shaders/Sprite");

    /// <summary>
    /// Loads the base sprite.
    /// </summary>
    /// <param name="offset">The offset in pixels from the top left.</param>
    /// <param name="tileSize">Size of each tile.</param>
    /// <param name="texName">Name of the texture.</param>
    /// <param name="shaderName">Name of the shader.</param>
    void Load(Vec2 offset, Vec2 tileSize, Ref<Texture> tex, StringView shaderName = "Shaders/Sprite");
    
    /// <summary>
    /// Returns the tile at the specified coordinates.
    /// </summary>
    /// <param name="x">The X coordinate.</param>
    /// <param name="y">The Y coordinate.</param>
    /// <returns>A pointer to the base sprite which now has the appropiate subtexture.</returns>
    Sprite* Get(int x, int y);

    SpriteAtlas::SpriteAtlasRow operator[](int y) {
        return SpriteAtlasRow(y, *this);
    }

    Sprite baseSprite;
    Vec2 offset;
    Vec2 tileSize;
    Vec2 scale = Vec2(1.0f);
};

}
