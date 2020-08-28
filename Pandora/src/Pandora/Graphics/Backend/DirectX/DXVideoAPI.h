#if !defined(PD_NO_DIRECTX)
#pragma once

#include "Pandora/Graphics/VideoAPI.h"

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

    ID3D11Device* GetDevice() const;
    ID3D11DeviceContext* GetDeviceContext() const;
    ID3D11RenderTargetView* GetBackBuffer() const;

protected:
    virtual void Init() override;

private:
    void UnbindAllTextures();

    Color clearColor;

    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;

    IDXGISwapChain* swapChain = nullptr;
    ID3D11RenderTargetView* target = nullptr;
    ID3D11RasterizerState* rasterizer = nullptr;
    ID3D11BlendState* alphaBlend = nullptr;
    ID3D11DepthStencilState* depthState = nullptr;
    ID3D11Texture2D* depthStencilTex = nullptr;
    ID3D11DepthStencilView* depthStencilView = nullptr;

    ID3D11InputLayout* vertexLayout = nullptr;

    bool vsyncEnabled = false;
};

}

#endif
