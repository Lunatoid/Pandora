#if !defined(PD_NO_OPENGL)
#pragma once

#include "Pandora/Graphics/ConstantBuffer.h"

#include "Pandora/Graphics/Backend/OpenGL/GLVideoAPI.h"

namespace pd {

class GLConstantBuffer : public ConstantBuffer {
public:
    GLConstantBuffer();

    virtual void SetBindTarget(ShaderType type) override;
    virtual void SetBindSlot(int slot) override;

    virtual void SetStruct(Slice<byte> bytes) override;

    virtual void SetElement(StringView name, bool data) override;
    virtual void SetElement(StringView name, f32 data) override;
    virtual void SetElement(StringView name, Vec2 data) override;
    virtual void SetElement(StringView name, Vec3 data) override;
    virtual void SetElement(StringView name, Vec4 data) override;
    virtual void SetElement(StringView name, int data) override;
    virtual void SetElement(StringView name, Vec2i data) override;
    virtual void SetElement(StringView name, Vec3i data) override;
    virtual void SetElement(StringView name, Vec4i data) override;
    virtual void SetElement(StringView name, Mat4 data) override;

    virtual void Upload() override;

private:
    void CheckActiveShader();

    Ref<Renderer> renderer;
    Ref<Shader> shader;
};

}

#endif
