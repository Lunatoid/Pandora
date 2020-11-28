#if !defined(PD_NO_DIRECTX)
#pragma once

#include <d3d11.h>

#include "Pandora/Graphics/FrameBuffer.h"

#include "Pandora/Graphics/Backend/DirectX/DXVideoAPI.h"

namespace pd {

class DXFrameBuffer final : public FrameBuffer {
public:
    virtual ~DXFrameBuffer() = default;

    virtual void Create(Vec2i size) override;
    virtual void Delete() override;

    virtual void Bind() override;
    virtual void BindAndClear(Color clearColor = Color(0.0f, 0.0f)) override;

private:
    DXVideoAPI* video = nullptr;
    DXComRef<ID3D11RenderTargetView> renderTarget;
    DXComRef<ID3D11DepthStencilView> depthStencilView;
};

}

#endif
