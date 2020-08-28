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

    /// <summary>
    /// Attaches this texture to the currently bound frame buffer.
    /// If <c>asColor</c> is true it will attach it as a color buffer, otherwise it will attach as a depth/stencil buffer.
    /// </summary
    /// <param name="fbo">The ID of the frame buffer object.</param>
    /// <param name="size">The size of the frame buffer in pixels.</param>
    /// <param name="asColor">Whether or not to attach it as a color buffer or depth/stencil buffer.</param>
    void AttachFramebuffer(u32 fbo, Vec2i size, bool asColor = true);

#if !defined(PD_NO_IMGUI)
    virtual ImTextureID GetImGuiTextureID() override;
#endif

private:
    void CreateTextureData();

    u32 id = 0;
};

}

#endif
