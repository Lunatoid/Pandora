#if !defined(PD_NO_DIRECTX)
#include "DXFrameBuffer.h"

#include "Pandora/Graphics/Backend/DirectX/DXTexture.h"

namespace pd {

void DXFrameBuffer::Create(Vec2i size) {
    video = (DXVideoAPI*)VideoAPI::Get();

    D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc = {};
    renderTargetDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    // Create color buffer
    colorBuffer = video->CreateTexture();
    colorBuffer.As<DXTexture>()->CreateRenderTarget(&renderTargetDesc, &renderTarget.AsOut(), size);

    // Create depth/stencil buffer
    depthStencilBuffer = video->CreateTexture();
    depthStencilBuffer.As<DXTexture>()->CreateDepthStencil(&depthStencilDesc, &depthStencilView.AsOut(), size);

    video->BindDefaultFrameBuffer();

}

void DXFrameBuffer::Delete() {
    renderTarget.Reset();
    depthStencilBuffer.Reset();
    depthStencilView.Reset();
}

void DXFrameBuffer::Bind() {
    video->GetDeviceContext()->OMSetRenderTargets(1, &renderTarget.Get(), depthStencilView.Get());

    Vec2i size = colorBuffer->GetSize();
    VideoAPI::Get()->SetViewport(size);
}

void DXFrameBuffer::BindAndClear(Color clearColor) {
    Bind();
    video->GetDeviceContext()->ClearRenderTargetView(renderTarget.Get(), clearColor.elements);
    video->GetDeviceContext()->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

}

#endif
