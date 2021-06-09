#if !defined(PD_NO_OPENGL)
#pragma once

#include "Pandora/Graphics/FrameBuffer.h"

namespace pd {

class GLFrameBuffer final : public FrameBuffer {
public:
    virtual ~GLFrameBuffer();

    virtual void Create(Vec2i size) override;
    virtual void Delete() override;

    virtual void Bind() override;
    virtual void BindAndClear(Color clearColor = Color(0.0f, 0.0f)) override;

private:
    u32 fbo = 0;
};

}

#endif
