#if !defined(PD_NO_DIRECTX)
#pragma once

#include <d3d11.h>

#include "Pandora/Graphics/Texture.h"

#include "Pandora/Graphics/Backend/DirectX/DXVideoAPI.h"

namespace pd {

class DXTexture final : public Texture {
public:
    DXTexture();
    virtual ~DXTexture();

    virtual void Delete() override;
    virtual void Upload(bool releasePixelData = true) override;
    virtual void Bind(int textureSlot = 0) override;

    virtual bool ShouldFlipVertically() override;

#if !defined(PD_NO_IMGUI)
    virtual ImTextureID GetImGuiTextureID() override;
#endif

    void CreateRenderTarget(D3D11_RENDER_TARGET_VIEW_DESC* renderTargetDesc,
        ID3D11RenderTargetView** renderTarget, Vec2i size);

    void CreateDepthStencil(D3D11_DEPTH_STENCIL_VIEW_DESC* depthStencilDesc,
        ID3D11DepthStencilView** depthStencilView, Vec2i size);

private:
    virtual void CreateTextureData() override;

    DXVideoAPI* video = nullptr;
    DXComRef<ID3D11Texture2D> texture;
    DXComRef<ID3D11ShaderResourceView> textureView;
    DXComRef<ID3D11SamplerState> samplerState;

    // @HACK:
    // This is kinda a hacky workaround to get
    // the correct bind flags to work for render targets
    u32 bindFlags = D3D11_BIND_SHADER_RESOURCE;
    DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
    bool flipVertically = false;
};

}

#endif
