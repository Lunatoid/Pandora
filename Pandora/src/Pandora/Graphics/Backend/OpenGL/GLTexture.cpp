#if !defined(PD_NO_OPENGL)
#include "GLTexture.h"

#include "Pandora/Core/Data/Hash.h"

#include "Pandora/Core/IO/File.h"
#include "Pandora/Core/IO/Console.h"

#include "Pandora/Libs/stb/stb_image.h"

#include "Pandora/Libs/glad/glad.h"

namespace pd {

GLTexture::~GLTexture() {
    Delete();
}

void GLTexture::Delete() {
    if (id != 0) {
        glDeleteTextures(1, &id);
    }

    if (pixels) {
        Free(pixels);
        pixels = nullptr;
    }
}

void GLTexture::Upload(bool releasePixelData) {
    if (id == 0 || !pixels) return;

    hash = DoHash(pixels, (u64)size.x * (u64)size.y * 4);

    // Upload it to the GPU
    glBindTexture(GL_TEXTURE_2D, id);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 size.x, size.y, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, pixels);

    glGenerateMipmap(GL_TEXTURE_2D);

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (releasePixelData) {
        ReleasePixelData();
    }
}

void GLTexture::Bind(int textureSlot) {
    if (id == 0) return;

    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_2D, id);
}

u32 GLTexture::GetID() const {
    return id;
}

// The asColor thing feels dirty...
void GLTexture::AttachFramebuffer(u32 fbo, Vec2i size, bool asColor) {
    CreateTextureData();

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    if (asColor) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8,
                     size.x, size.y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, id, 0);
    }

    this->size = size;

    // We still need a unique hash so let's hash the size and fbo
    // Hopefully that will generate unique enough hashes
    struct {
        u32 fbo;
        Vec2i size;
    } hashData = { fbo, size };

    hash = DoHash(hashData);
}

bool GLTexture::ShouldFlipVertically() {
    return false;
}

#if !defined(PD_NO_IMGUI)
ImTextureID GLTexture::GetImGuiTextureID() {
    return (ImTextureID)(u64)GetID();
}
#endif

void GLTexture::CreateTextureData() {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    GLint wrapMode = 0;
    switch (wrapping) {
        case TextureWrapping::Clamp:
            wrapMode = GL_CLAMP_TO_EDGE;
            break;

        case TextureWrapping::Repeat:
            wrapMode = GL_REPEAT;
            break;

        default:
            PD_ASSERT_D(false, "Unhandled texture wrapping mode %d", wrapping);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);

    GLint minFilterMode = GL_INVALID_VALUE;
    GLint magFilterMode = GL_INVALID_VALUE;
    switch (filtering) {
        case TextureFiltering::Point:
            minFilterMode = GL_NEAREST;
            magFilterMode = GL_NEAREST;
            break;

        case TextureFiltering::Bilinear:
            minFilterMode = GL_LINEAR;
            magFilterMode = GL_LINEAR;
            break;

        case TextureFiltering::Anisotropic:
            if (GLAD_GL_EXT_texture_filter_anisotropic) {
                f32 largestSupportedAnitropy = 0.0f;
                glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &largestSupportedAnitropy);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, largestSupportedAnitropy);
            } else {
                CONSOLE_LOG_DEBUG("[{}GL Error{}] GL_EXT_texture_filter_anisotropic is not supported\n", ConColor::Red, ConColor::White);
            }
            // Fall through to Trilinear

        case TextureFiltering::Trilinear:
            minFilterMode = GL_LINEAR_MIPMAP_LINEAR;
            magFilterMode = GL_LINEAR;
            break;

        default:
            PD_ASSERT_D(false, "Unhandled texture filtering mode %d", filtering);
    }

    if (minFilterMode != GL_INVALID_VALUE) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilterMode);
    }

    if (magFilterMode != GL_INVALID_VALUE) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilterMode);
    }
}

}

#endif
