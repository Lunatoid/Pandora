#if !defined(PD_NO_OPENGL)
#pragma once

#include "Pandora/Graphics/Texture.h"

namespace pd {

class GLTexture final : public Texture {
public:
    virtual ~GLTexture();

    virtual void Delete() override;
    virtual void Upload(bool releasePixelData = true) override;
    virtual void Bind(int textureSlot = 0) override;

    u32 GetID() const;

    /**
     * \brief Attaches this texture to the currently bound frame buffer.
     * If `asColor` is true it will attach as a color buffer, otherwise it will be attached
     * as a depth/stencil buffer.
     * 
     * \param fbo The ID of the frame buffer.
     * \param size The size of the frame buffer.
     * \param asColor Whether it should attach as color or as depth/stencil.
     */
    void AttachFramebuffer(u32 fbo, Vec2i size, bool asColor = true);

    virtual bool ShouldFlipVertically();

#if !defined(PD_NO_IMGUI)
    virtual ImTextureID GetImGuiTextureID() override;
#endif

private:
    void CreateTextureData();

    u32 id = 0;
};

}

#endif
