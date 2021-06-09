#include "SpriteAtlas.h"

namespace pd {

void SpriteAtlas::Load(Vec2 offset, Vec2 tileSize, StringView texName, StringView shaderName) {
    this->offset = offset;
    this->tileSize = tileSize;

    baseSprite.Load(texName, shaderName);
}

void SpriteAtlas::Load(Vec2 offset, Vec2 tileSize, Ref<Texture> tex, StringView shaderName) {
    this->offset = offset;
    this->tileSize = tileSize;

    baseSprite.Load(tex, shaderName);
}

Sprite* SpriteAtlas::Get(int x, int y) {
    baseSprite.SetAtlasMask(Vec2i(x, y), tileSize, offset);
    baseSprite.scale = (tileSize / baseSprite.size) * scale;

    return &baseSprite;
}

}
