#pragma once

#include "Pandora/Graphics/Sprite/Sprite.h"

namespace pd {

// @TODO: this class is not that good or useful. Improve it, or remove it.
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

    /**
     * \brief Loads the base atlas sprite.
     * 
     * \param offset The offset in pixels from the top left.
     * \param tileSize The size of each tile.
     * \param texName The texture resource name.
     * \param shaderName The shader resource name.
     */
    void Load(Vec2 offset, Vec2 tileSize, StringView texName, StringView shaderName = "Shaders/Sprite");

    /**
     * \brief Loads the base atlas sprite.
     * 
     * \param offset The offset in pixels from the top left.
     * \param tileSize The size of each tile.
     * \param tex The texture.
     * \param shaderName The shader resource name.
     */
    void Load(Vec2 offset, Vec2 tileSize, Ref<Texture> tex, StringView shaderName = "Shaders/Sprite");

    /**
     * \param x The x-coordinate from the top left.
     * \param y The y-coordinate from the top left.
     * \return A pointer to the base sprite with the appropiate clipping mask.
     */
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
