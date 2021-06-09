#if !defined(PD_NO_OPENGL)
#pragma once

#include "Pandora/Graphics/Renderer.h"

#include "Pandora/Graphics/Backend/OpenGL/GLBuffer.h"

namespace pd {

class GLRenderer final : public Renderer {
public:
    GLRenderer();
    ~GLRenderer();

    virtual void Submit(int indexOffset = 0, int indexCount = -1, bool clearData = true) override;

private:
    u32 vao = 0;
    GLBuffer vbo;
    GLBuffer ibo;

};

}

#endif
