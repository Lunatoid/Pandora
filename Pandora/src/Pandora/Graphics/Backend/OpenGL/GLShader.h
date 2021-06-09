#if !defined(PD_NO_OPENGL)
#pragma once

#include "Pandora/Core/Math/Matrix.h"
#include "Pandora/Core/Math/Vector.h"
#include "Pandora/Core/Math/VectorInt.h"

#include "Pandora/Graphics/Shader.h"

namespace pd {

class GLShader final : public Shader {
public:
    virtual ~GLShader();

    virtual bool Load(StringView name) override;
    virtual bool Load(Box& box, StringView name) override;
    virtual bool Load(StringView vertexPath, StringView pixelPath) override;

    virtual void SetLayout(DataLayout& layout) override;
    virtual DataLayout& GetLayout() override;

    virtual void Bind() override;
    virtual void ActivateTextureSlot(int slot) override;

    virtual Slice<byte> GetVertexData() override;
    virtual Slice<byte> GetPixelData() override;

    int GetLocation(StringView name);
    void SetUniform(StringView name, bool data);
    void SetUniform(StringView name, f32 data);
    void SetUniform(StringView name, Vec2 data);
    void SetUniform(StringView name, Vec3 data);
    void SetUniform(StringView name, Vec4 data);
    void SetUniform(StringView name, int data);
    void SetUniform(StringView name, Vec2i data);
    void SetUniform(StringView name, Vec3i data);
    void SetUniform(StringView name, Vec4i data);
    void SetUniform(StringView name, Mat4 data);

private:
    void CreateFromFile();
    void ActivateLayout();

    String vertexFile;
    String pixelFile;

    u32 programID = 0;
    u32 vertexID = 0;
    u32 pixelID = 0;
};

}

#endif
