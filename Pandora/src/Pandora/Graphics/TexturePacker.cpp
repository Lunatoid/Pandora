#include "TexturePacker.h"

#include "Pandora/Core/Resources/ResourceCatalog.h"

#include "Pandora/Graphics/VideoAPI.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include "Pandora/Libs/stb/stb_rect_pack.h"

namespace pd {

TexturePacker::TexturePacker(int size, int padding, TextureFiltering filtering, TextureWrapping wrapping) :
    textureSize(size), padding(padding), filtering(filtering), wrapping(wrapping) {}

bool TexturePacker::PackTexture(const Ref<Texture>& texture) {
    Vec4 uv;
    return PackTexture(texture, uv);
}

bool TexturePacker::PackTexture(const Ref<Texture>& texture, Vec4& uv) {
    Vec2i size = texture->GetSize();
    Vec2i position;

    if (PackRect(size, position, uv)) {
        base->SetPixels(position, texture);
        base->Upload(false);
        return true;
    }

    return false;
}

bool TexturePacker::PackTexture(StringView textureName) {
    Vec4 uv;
    return PackTexture(textureName, uv);
}

bool TexturePacker::PackTexture(StringView textureName, Vec4& uv) {
    return PackTexture(ResourceCatalog::Get().Get<Texture>(textureName), uv);
}

bool TexturePacker::PackTexture(Slice<byte> pixels, int stride) {
    Vec4 uv;
    return PackTexture(pixels, stride, uv);
}

bool TexturePacker::PackTexture(Slice<byte> pixels, int stride, Vec4& uv) {
    Vec2i size = Vec2i(stride, pixels.Count() / stride * 4);
    Vec2i position;

    if (PackRect(size, position, uv)) {
        base->SetPixels(position, pixels, stride);
        base->Upload(false);
        return true;
    }

    return false;
}

bool TexturePacker::PackTexture(Slice<Color> pixels, int stride) {
    Vec4 uv;
    return PackTexture(pixels, stride, uv);
}

bool TexturePacker::PackTexture(Slice<Color> pixels, int stride, Vec4& uv) {
    Vec2i size = Vec2i(stride, pixels.Count() / stride);
    Vec2i position;

    if (PackRect(size, position, uv)) {
        base->SetPixels(position, pixels, stride);
        base->Upload(false);
        return true;
    }

    return false;
}

Ref<Texture> TexturePacker::GetTexture(RefType type) {
    return base.NewRef(type);
}

void TexturePacker::Init() {
    if (!base) {
        base = VideoAPI::Get()->CreateTexture();
        base->filtering = filtering;
        base->wrapping = wrapping;
        base->Create(Vec2i(textureSize, textureSize), false);

        nodes.Reserve(textureSize * 2);
        stbrp_init_target(&context, textureSize, textureSize, nodes.Data(), nodes.Count());
    }
}

bool TexturePacker::PackRect(Vec2i size, Vec2i& position, Vec4& uv) {
    Init();

    stbrp_rect rect = {};
    rect.w = size.x + padding;
    rect.h = size.y + padding;

    stbrp_pack_rects(&context, &rect, 1);

    position = Vec2i(rect.x, rect.y);

    uv = Vec4((f32)rect.x / (f32)base->GetSize().x,
              (f32)rect.y / (f32)base->GetSize().y,
              (f32)size.x / (f32)base->GetSize().x,
              (f32)size.y / (f32)base->GetSize().y);

    return rect.was_packed;
}

}
