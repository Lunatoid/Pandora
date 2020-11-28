#pragma once

#include "Pandora/Graphics/Texture.h"

#include "Pandora/Libs/stb/stb_rect_pack.h"

namespace pd {

class TexturePacker {
public:
    TexturePacker(int size = 4096, int padding = 0,
                  TextureFiltering filtering = TextureFiltering::Anisotropic,
                  TextureWrapping wrapping = TextureWrapping::Clamp);
    ~TexturePacker() {}

    bool PackTexture(const Ref<Texture>& texture);
    bool PackTexture(const Ref<Texture>& texture, Vec4& uv);

    bool PackTexture(StringView textureName);
    bool PackTexture(StringView textureName, Vec4& uv);

    bool PackTexture(Slice<byte> pixels, int stride);
    bool PackTexture(Slice<byte> pixels, int stride, Vec4& uv);

    bool PackTexture(Slice<Color> pixels, int stride);
    bool PackTexture(Slice<Color> pixels, int stride, Vec4& uv);

    Ref<Texture> GetTexture(RefType type = RefType::Strong);

private:
    void Init();
    bool PackRect(Vec2i size, Vec2i& position, Vec4& uv);

    int textureSize;
    int padding;
    TextureFiltering filtering;
    TextureWrapping wrapping;

    Ref<Texture> base;
    stbrp_context context = {};
    Array<stbrp_node> nodes;
};

}
