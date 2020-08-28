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
    Vec2 texSize = baseSprite.GetSize(0, false);
    Vec2 maxIndex = texSize / tileSize;

    if (x > 0) {
        x %= (int)maxIndex.x;
    }

    if (y > 0) {
        y %= (int)maxIndex.y;
    }

    Vec2 start = offset + (Vec2((f32)x, (f32)y + 1.0f) * tileSize);
    start.y = texSize.y - start.y;

    baseSprite.SetClippingMask(Vec4(start.x, start.y, tileSize.x, tileSize.y));
    baseSprite.scale = (tileSize / baseSprite.size) * scale;

    return &baseSprite;
}

}
