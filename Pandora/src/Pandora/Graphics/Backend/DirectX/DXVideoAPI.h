#if !defined(PD_NO_DIRECTX)
#pragma once

#include "Pandora/Graphics/VideoAPI.h"

#include "Pandora/Graphics/Backend/DirectX/DXComRef.h"

// Forward declare all this dumb shit because I really don't want
// to polute the global namespace with all the dumbass Windows macros
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11RasterizerState;
struct ID3D11BlendState;
struct ID3D11DepthStencilState;
struct ID3D11Texture2D;
struct ID3D11DepthStencilView;
struct ID3D11InputLayout;

namespace pd {

bool CheckDXError(long result);

class DXVideoAPI final : public VideoAPI {
public:
    DXVideoAPI(Window* window);
    virtual ~DXVideoAPI();

    virtual void SetClearColor(Color color) override;
    virtual void Clear() override;
    virtual void Swap() override;
    virtual void SetVsync(bool enabled) override;
    virtual void SetViewport(Vec2i size) override;
    virtual void BindDefaultFrameBuffer() override;

#if !defined(PD_NO_IMGUI)
    virtual void ImGuiNewFrame() override;
    virtual void ImGuiRender() override;
#endif

    virtual Ref<Shader> CreateShader() override;
    virtual Ref<Texture> CreateTexture() override;
    virtual Ref<Font> CreateFont() override;
    virtual Ref<FrameBuffer> CreateFrameBuffer() override;

    virtual Ref<ConstantBuffer> CreateConstantBuffer() override;

    virtual void SetTextureRequestHandler(ResourceCatalog& catalog) override;
    virtual void SetShaderRequestHandler(ResourceCatalog& catalog) override;
    void DXVideoAPI::SetFontRequestHandler(ResourceCatalog& catalog) override;

    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetDeviceContext();
    ID3D11RenderTargetView* GetBackBuffer();

protected:
    virtual void Init() override;

private:
    void UnbindAllTextures();

    DXComRef<ID3D11Device> device;
    DXComRef<ID3D11DeviceContext> context;

    DXComRef<IDXGISwapChain> swapChain;
    DXComRef<ID3D11RenderTargetView> target;
    DXComRef<ID3D11RasterizerState> rasterizer;
    DXComRef<ID3D11BlendState> alphaBlend;
    DXComRef<ID3D11DepthStencilState> depthState;
    DXComRef<ID3D11Texture2D> depthStencilTex;
    DXComRef<ID3D11DepthStencilView> depthStencilView;

    DXComRef<ID3D11InputLayout> vertexLayout;

    bool vsyncEnabled = false;
};

}

#endif
