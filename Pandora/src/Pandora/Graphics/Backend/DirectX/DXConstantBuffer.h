#if !defined(PD_NO_DIRECTX)
#pragma once

#include "Pandora/Graphics/ConstantBuffer.h"

#include "Pandora/Graphics/Backend/DirectX/DXVideoAPI.h"
#include "Pandora/Graphics/Backend/DirectX/DXBuffer.h"

namespace pd {

class DXConstantBuffer : public ConstantBuffer {
public:
    DXConstantBuffer();

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
    /// <summary>
    /// Sets the element in the bytes array.
    /// </summary>
    /// <param name="offset">The offset.</param>
    /// <param name="data">The data.</param>
    void SetElement(int offset, Slice<byte> data);
    
    /// <summary>
    /// Finds the offset in bytes of an element by name.
    /// </summary>
    /// <param name="name">The name.</param>
    /// <returns>The offset.</returns>
    int FindElementOffset(StringView name);
    
    /// <summary>
    /// Makes sure <c>bytes</c> is as large as the layout stride.
    /// </summary>
    void CheckLayoutSize();

    DXVideoAPI* video = nullptr;
    Array<byte> bytes;

    int slot = 0;
    ShaderType bindTarget = ShaderType::Vertex;

    DXBuffer internalBuffer;
};

}

#endif
