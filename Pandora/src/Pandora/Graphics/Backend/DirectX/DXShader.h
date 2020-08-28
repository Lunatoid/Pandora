#if !defined(PD_NO_DIRECTX)
#pragma once

#include <d3d11.h>

#include "Pandora/Core/Data/Array.h"

#include "Pandora/Graphics/Shader.h"


namespace pd {

class DXVideoAPI;

class DXShader final : public Shader {
public:
    DXShader();
    virtual ~DXShader();

    virtual bool Load(StringView name) override;
    virtual bool Load(Box& box, StringView name) override;
    virtual bool Load(StringView vertexPath, StringView pixelPath) override;

    virtual void SetLayout(DataLayout& layout) override;
    virtual DataLayout& GetLayout() override;

    virtual void Bind() override;
    virtual void ActivateTextureSlot(int slot) override;

    virtual Slice<byte> GetVertexData() override;
    virtual Slice<byte> GetPixelData() override;

private:
    void CreateFromBytecode();

    DXVideoAPI* video = nullptr;

    ID3D11VertexShader* vertexShader = nullptr;
    ID3D11PixelShader* pixelShader = nullptr;
    ID3D11InputLayout* vertexLayout = nullptr;

    Array<byte> vertexBytecode;
    Array<byte> pixelBytecode;
};

}

#endif
