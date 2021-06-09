#if !defined(PD_NO_OPENGL)
#include "GLFrameBuffer.h"

#include "Pandora/Graphics/VideoAPI.h"

#include "Pandora/Graphics/Backend/OpenGL/GLTexture.h"

#include "Pandora/Libs/glad/glad.h"

namespace pd {

GLFrameBuffer::~GLFrameBuffer() {
    Delete();
}

void GLFrameBuffer::Create(Vec2i size) {
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Create and attach color buffer
    colorBuffer = VideoAPI::Get()->CreateTexture();
    colorBuffer.As<GLTexture>()->AttachFramebuffer(fbo, size);

    // Create and attach depth/stencil buffer
    depthStencilBuffer = VideoAPI::Get()->CreateTexture();
    depthStencilBuffer.As<GLTexture>()->AttachFramebuffer(fbo, size, false);

    // Unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLFrameBuffer::Delete() {
    glDeleteFramebuffers(1, &fbo);
}

void GLFrameBuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    VideoAPI::Get()->SetViewport(colorBuffer->GetSize());
}

void GLFrameBuffer::BindAndClear(Color clearColor) {
    Bind();

    VideoAPI* video = VideoAPI::Get();
    Color oldColor = video->GetClearColor();

    video->SetClearColor(clearColor);
    video->Clear();

    video->SetClearColor(oldColor);
}

}

#endif

