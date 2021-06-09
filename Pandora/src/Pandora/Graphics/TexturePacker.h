#pragma once

#include "Pandora/Graphics/Texture.h"

#include "Pandora/Libs/stb/stb_rect_pack.h"

namespace pd {

class TexturePacker {
public:
    /**
     * \brief Initializes the texture packer.
     * 
     * \param size The size of the texture texture.
     * \param padding How many pixels padding each texture should have.
     * \param filtering The filtering that should be used.
     * \param wrapping The wrapping that should be used.
     */
    TexturePacker(int size = 4096, int padding = 0,
                  TextureFiltering filtering = TextureFiltering::Anisotropic,
                  TextureWrapping wrapping = TextureWrapping::Clamp);
    ~TexturePacker() {}

    /**
     * \brief Adds the texture to the atlas.
     * 
     * \param texture The texture.
     * \return Whether or not the texture got packed successfully.
     */
    bool PackTexture(const Ref<Texture>& texture);

    /**
     * \brief Adds the texture to the atlas.
     * 
     * \param texture The texture.
     * \param uv The UV coordinates of the texture if it got added successfully.
     * \return Whether or not the texture got packed successfully.
     */
    bool PackTexture(const Ref<Texture>& texture, Vec4& uv);

    /**
     * \brief Adds the texture to the atlas.
     * 
     * \param textureName The texture resource name.
     * \return Whether or not the texture got packed successfully.
     */
    bool PackTexture(StringView textureName);

    /**
     * \brief Adds the texture to the atlas.
     * 
     * \param textureName The texture resource name.
     * \param uv The UV coordinates of the texture if it got added successfully.
     * \return Whether or not the texture got packed successfully.
     */
    bool PackTexture(StringView textureName, Vec4& uv);

    /**
     * \brief Adds the texture to the atlas.
     * 
     * \param pixels The pixel data in RGBA format.
     * \param stride The stride in pixels.
     * \return Whether or not the texture got packed successfully.
     */
    bool PackTexture(Slice<byte> pixels, int stride);

    /**
     * \brief Adds the texture to the atlas.
     * 
     * \param pixels The pixel data in RGBA format.
     * \param stride The stride in pixels.
     * \param uv The UV coordinates of the texture if it got added successfully.
     * \return Whether or not the texture got packed successfully.
     */
    bool PackTexture(Slice<byte> pixels, int stride, Vec4& uv);

    /**
     * \brief Adds the texture to the atlas.
     * 
     * \param pixels The pixel data.
     * \param stride The stride in pixels.
     * \return Whether or not the texture got packed successfully.
     */
    bool PackTexture(Slice<Color> pixels, int stride);

    /**
     * \brief Adds the texture to the atlas.
     * 
     * \param pixels The pixel data.
     * \param stride The stride in pixels.
     * \param uv The UV coordinates of the texture if it got added successfully.
     * \return Whether or not the texture got packed successfully.
     */
    bool PackTexture(Slice<Color> pixels, int stride, Vec4& uv);

    /**
     * \param type The reference type.
     * \return The new reference to the atlas texture.
     */
    Ref<Texture> GetTexture(RefType type = RefType::Strong);

private:

    /**
     * \brief Creates the atlas texture.
     */
    void Init();

    /**
     * \brief Attempts to pack the rect.
     * 
     * \param size The size of the rect.
     * \param position The output of the position it got packed.
     * \param uv The output of the UV of the rect.
     * \return Whether it got packed successfully.
     */
    bool PackRect(Vec2i size, Vec2i& position, Vec4& uv);

    int textureSize = 0;
    int padding = 0;
    TextureFiltering filtering = TextureFiltering::Anisotropic;
    TextureWrapping wrapping = TextureWrapping::Clamp;

    Ref<Texture> base;
    stbrp_context context = {};
    Array<stbrp_node> nodes;
};

}
