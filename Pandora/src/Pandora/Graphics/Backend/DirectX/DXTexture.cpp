#if !defined(PD_NO_DIRECTX)
#include "DXTexture.h"

#include "Pandora/Core/Data/Hash.h"

#include "Pandora/Core/IO/File.h"
#include "Pandora/Core/IO/Console.h"

#include "Pandora/Libs/stb/stb_image.h"

#include <d3d11.h>

namespace pd {

// @TODO: sRGB

DXTexture::DXTexture() : video((DXVideoAPI*)VideoAPI::Get()) {
    bindFlags = D3D11_BIND_SHADER_RESOURCE;
}

DXTexture::~DXTexture() {
    Delete();
}

void DXTexture::Delete() {
    if (pixels) {
        Free(pixels);
        pixels = nullptr;
    }
}

void DXTexture::Upload(bool releasePixelData) {
    if (!texture || !pixels) return;

    hash = DoHash(pixels, (u64)size.x * (u64)size.y * 4);

    // Upload it to the GPU
    D3D11_SUBRESOURCE_DATA data = {};
    data.pSysMem = pixels;
    data.SysMemPitch = size.x * 4;

    video->GetDeviceContext()->UpdateSubresource(texture.Get(), D3D11CalcSubresource(0, 0, 1), nullptr, pixels, size.x * 4, 0);

    if (releasePixelData) {
        ReleasePixelData();
    }
}

void DXTexture::Bind(int textureSlot) {
    if (!textureView) return;

    video->GetDeviceContext()->PSSetShaderResources(textureSlot, 1, &textureView.Get());
    video->GetDeviceContext()->PSSetSamplers(textureSlot, 1, &samplerState.Get());
}

bool DXTexture::ShouldFlipVertically() {
    // If it's a render target it needs to be flipped vertically
    return bindFlags & D3D11_BIND_RENDER_TARGET;
}

#if !defined(PD_NO_IMGUI)
ImTextureID DXTexture::GetImGuiTextureID() {
    return (ImTextureID)textureView.Get();
}
#endif

void DXTexture::CreateRenderTarget(D3D11_RENDER_TARGET_VIEW_DESC* renderTargetDesc, ID3D11RenderTargetView** renderTarget, Vec2i size) {
    this->size = size;

    bindFlags |= D3D11_BIND_RENDER_TARGET;
    CreateTextureData();

    CheckDXError(video->GetDevice()->CreateRenderTargetView(texture.Get(), renderTargetDesc, renderTarget));

    // Create unique hash
    struct {
        void* targetAddr;
        Vec2i size;
    } hashData = { renderTarget, size };

    hash = DoHash(&hashData);
}

void DXTexture::CreateDepthStencil(D3D11_DEPTH_STENCIL_VIEW_DESC* depthStencilDesc, ID3D11DepthStencilView** depthStencilView, Vec2i size) {
    this->size = size;

    bindFlags = D3D11_BIND_DEPTH_STENCIL;
    format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    CreateTextureData();

    CheckDXError(video->GetDevice()->CreateDepthStencilView(texture.Get(), depthStencilDesc, depthStencilView));

    // Create unique hash
    struct {
        void* targetAddr;
        Vec2i size;
    } hashData = { depthStencilView, size };

    hash = DoHash(&hashData);
}

void DXTexture::CreateTextureData() {
    const UINT MIP_LEVELS = 1;

    D3D11_TEXTURE2D_DESC texDesc = {};
    texDesc.Width = size.x;
    texDesc.Height = size.y;
    texDesc.Format = format;
    texDesc.MipLevels = MIP_LEVELS;
    texDesc.ArraySize = 1;
    texDesc.SampleDesc.Count = 1;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = bindFlags;

    CheckDXError(video->GetDevice()->CreateTexture2D(&texDesc, nullptr, &texture.AsOut()));

    if (bindFlags & D3D11_BIND_SHADER_RESOURCE) {
        D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
        viewDesc.Format = texDesc.Format;
        viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        viewDesc.Texture2D.MipLevels = MIP_LEVELS;

        CheckDXError(video->GetDevice()->CreateShaderResourceView(texture.Get(), &viewDesc, &textureView.AsOut()));
    }

    D3D11_TEXTURE_ADDRESS_MODE wrapMode = D3D11_TEXTURE_ADDRESS_CLAMP;
    switch (wrapping) {
        case TextureWrapping::Clamp:
            wrapMode = D3D11_TEXTURE_ADDRESS_CLAMP;
            break;

        case TextureWrapping::Repeat:
            wrapMode = D3D11_TEXTURE_ADDRESS_WRAP;
            break;

        default:
            PD_ASSERT_D(false, "Unhandled texture wrapping mode %d", wrapping);
    }

    D3D11_SAMPLER_DESC samplerDesc = {};
    switch (filtering) {
        case TextureFiltering::Point:
            samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
            break;

        case TextureFiltering::Bilinear:
            samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
            break;

        case TextureFiltering::Trilinear:
            samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            break;

        case TextureFiltering::Anisotropic:
            samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
            break;

        default:
            PD_ASSERT_D(false, "Unhandled texture filtering mode %d", filtering);
    }

    samplerDesc.AddressU = wrapMode;
    samplerDesc.AddressV = wrapMode;
    samplerDesc.AddressW = wrapMode;

    CheckDXError(video->GetDevice()->CreateSamplerState(&samplerDesc, &samplerState.AsOut()));
}

}

#endif
