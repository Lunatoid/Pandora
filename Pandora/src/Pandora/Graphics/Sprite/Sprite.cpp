#include "Sprite.h"

namespace pd {

void GetSpriteVertexLayout(DataLayout& layout) {
    layout.Add("vertPos", DataLayoutType::Float3);
    layout.Add("position", DataLayoutType::Float3);
    layout.Add("color", DataLayoutType::Float4);
    layout.Add("uv", DataLayoutType::Float2);
    layout.Add("pivot", DataLayoutType::Float2);
    layout.Add("rotation", DataLayoutType::Float);
    layout.Add("size", DataLayoutType::Float2);
    layout.Finish();
}

Sprite::Sprite(Sprite& other) {
    position = other.position;
    color = other.color;
    size = other.size;
    scale = other.scale;
    pivot = other.pivot;
    rotation = other.rotation;
    material = other.material;
    uv = other.uv;
}

void Sprite::GenerateVertices(SpriteVertex vertices[4]) {
    Vec2 finalSize = size * scale;

    Vec4 uv = this->uv;

    // This may not be the most elegant way but it
    // ensures that frame buffers are all the right way.
    if (material->GetTexture(0)->ShouldFlipVertically()) {
        uv.y = 1.0f - uv.y;
        uv.w *= -1.0f;
    }

    vertices[0].vertPos = position;
    vertices[0].position = position;
    vertices[0].color = color;
    vertices[0].uv = Vec2(uv.x, uv.y);
    vertices[0].pivot = pivot;
    vertices[0].rotation = rotation;
    vertices[0].size = finalSize;

    vertices[1].vertPos = position + Vec3(finalSize.x, 0.0f, 0.0f);
    vertices[1].position = position;
    vertices[1].color = color;
    vertices[1].uv = Vec2(uv.x + uv.z, uv.y);
    vertices[1].pivot = pivot;
    vertices[1].rotation = rotation;
    vertices[1].size = finalSize;

    vertices[2].vertPos = position + Vec3(0.0f, finalSize.y, 0.0f);
    vertices[2].position = position;
    vertices[2].color = color;
    vertices[2].uv = Vec2(uv.x, uv.y + uv.w);
    vertices[2].pivot = pivot;
    vertices[2].rotation = rotation;
    vertices[2].size = finalSize;

    vertices[3].vertPos = position + Vec3(finalSize);
    vertices[3].position = position;
    vertices[3].color = color;
    vertices[3].uv = Vec2(uv.x + uv.z, uv.y + uv.w);
    vertices[3].pivot = pivot;
    vertices[3].rotation = rotation;
    vertices[3].size = finalSize;
}

void Sprite::SetMaterial(StringView shaderName, bool clearTextures) {
    material->Load(shaderName, material->GetLayout());
    if (clearTextures) {
        material->ClearTextures();
    }
}

Material* Sprite::GetMaterial() {
    return material.Get();
}

void Sprite::SetClippingMask(Vec4 mask) {
    Vec2 texSize = (Vec2)GetSize(0, false);
    uv = Vec4(mask.x / texSize.x, mask.y / texSize.y, mask.z / texSize.x, mask.w / texSize.y);
}

void Sprite::SetNormalizedClippingMask(Vec4 mask) {
    uv = mask;
}

void Sprite::SetAtlasMask(Vec2i position, Vec2 tileSize, Vec2 offset) {
    Vec2 texSize = GetSize(0, false);
    Vec2 maxIndex = texSize / tileSize;

    if (maxIndex.x > 0.0f && position.x > 0) {
        position.x %= (int)maxIndex.x;
    }

    if (maxIndex.y > 0.0f && position.y > 0) {
        position.y %= (int)maxIndex.y;
    }

    Vec2 start = offset + (Vec2((f32)position.x, (f32)position.y + 1.0f) * tileSize);
    start.y = texSize.y - start.y;

    SetClippingMask(Vec4(start.x, start.y, tileSize.x, tileSize.y));
}

Vec4 Sprite::GetClippingMask() {
    return uv;
}

Vec2 Sprite::GetSize(int slot, bool applyScale) const {
    Vec2 texSize = material->GetTexture(slot, RefType::Weak)->GetSize();

    if (applyScale) {
        texSize.x *= scale.x;
        texSize.y *= scale.y;
    }

    return texSize;
}

u64 Sprite::GetMaterialHash() {
    return material->GetHash();
}

u64 Sprite::GetTextureHash(int slot) {
    return material->GetTextureHash(slot);
}

u64 Sprite::GetTextureHashes() {
    return material->GetTextureHashes();
}

}
