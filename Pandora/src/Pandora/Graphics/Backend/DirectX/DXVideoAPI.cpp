#if !defined(PD_NO_DIRECTX)
#include "DXVideoAPI.h"

#include "Pandora/Core/IO/Console.h"

#include "Pandora/Graphics/Backend/DirectX/DXShader.h"
#include "Pandora/Graphics/Backend/DirectX/DXRenderer.h"
#include "Pandora/Graphics/Backend/DirectX/DXTexture.h"
#include "Pandora/Graphics/Backend/DirectX/DXConstantBuffer.h"
#include "Pandora/Graphics/Backend/DirectX/DXFrameBuffer.h"

#include "Pandora/Graphics/Backend/FreeType/FTFont.h"

#include "Pandora/Libs/dxerr/dxerr.h"
#include "Pandora/Libs/ImGui/DirectX/imgui_impl_dx11.h"

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

namespace pd {

bool CheckDXError(long result) {
    bool failed = FAILED(result);
    if (failed) {
        wchar error[512];

        DXGetErrorDescriptionW(result, error, sizeof(error) / sizeof(wchar));

        PD_ASSERT_D(!failed, "%s\n\n%s", WideToUTF8(DXGetErrorStringW(result)), WideToUTF8(error));
    }

    return failed;
}

DXVideoAPI::DXVideoAPI(Window* window) :
    VideoAPI(window) {

    DXGI_SWAP_CHAIN_DESC swapDesc = {};
    swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapDesc.SampleDesc.Count = 1;
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapDesc.BufferCount = 2;
    swapDesc.OutputWindow = (HWND)window->GetNativeHandle();
    swapDesc.Windowed = !window->IsFullscreen();
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

#if defined(PD_DEBUG)
    u32 flags = D3D11_CREATE_DEVICE_DEBUG;
#else
    u32 flags = 0;
#endif

    CheckDXError(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
        nullptr, flags, nullptr, 0,
        D3D11_SDK_VERSION,
        &swapDesc,
        &swapChain.AsOut(),
        &device.AsOut(),
        nullptr,
        &context.AsOut()));

    // Get RenderTargetView
    ID3D11Resource* backBuffer = nullptr;
    CheckDXError(swapChain->GetBuffer(0, __uuidof(ID3D11Resource), (void**)&backBuffer));
    CheckDXError(device->CreateRenderTargetView(backBuffer, nullptr, &target.AsOut()));

    backBuffer->Release();

    // Set the culling mode to be consistent with OpenGL
    D3D11_RASTERIZER_DESC rasterDesc = {};
    rasterDesc.CullMode = D3D11_CULL_NONE; // BACK
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    rasterDesc.FrontCounterClockwise = true;
    rasterDesc.DepthClipEnable = true;

    CheckDXError(device->CreateRasterizerState(&rasterDesc, &rasterizer.AsOut()));

    context->RSSetState(rasterizer.Get());

    // Enable alpha blending
    D3D11_BLEND_DESC blendDesc = {};

    blendDesc.RenderTarget[0].BlendEnable = true;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;

    CheckDXError(device->CreateBlendState(&blendDesc, &alphaBlend.AsOut()));
    context->OMSetBlendState(alphaBlend.Get(), nullptr, 0xFFFFFFFF);

    // Enable depth-testing
    D3D11_DEPTH_STENCIL_DESC stencilDesc = {};
    stencilDesc.DepthEnable = true;
    stencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    stencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    CheckDXError(device->CreateDepthStencilState(&stencilDesc, &depthState.AsOut()));
    context->OMSetDepthStencilState(depthState.Get(), 1);

    // @TODO: switch to eventually using our FrameBuffer class
    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC depthTex = {};
    depthTex.Width = window->GetSize().x;
    depthTex.Height = window->GetSize().y;
    depthTex.MipLevels = 1;
    depthTex.ArraySize = 1;
    depthTex.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthTex.SampleDesc.Count = 1;
    depthTex.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    CheckDXError(device->CreateTexture2D(&depthTex, nullptr, &depthStencilTex.AsOut()));

    // Bind the depth and backbuffer
    D3D11_DEPTH_STENCIL_VIEW_DESC depthViewDesc = {};
    depthViewDesc.Format = depthTex.Format;
    depthViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    CheckDXError(device->CreateDepthStencilView(depthStencilTex.Get(), &depthViewDesc, &depthStencilView.AsOut()));

#if !defined(PD_NO_IMGUI)
    // Initialize ImGui
    ImGui_ImplDX11_Init(device.Get(), context.Get());
#endif
}

#define DX_RELEASE(com) if (com) {\
    com->Release();\
    com = nullptr;\
}

DXVideoAPI::~DXVideoAPI() {
#if !defined(PD_NO_IMGUI)
    ImGui_ImplDX11_Shutdown();
#endif
}

void DXVideoAPI::SetClearColor(Color color) {
    clearColor = color;
}

void DXVideoAPI::Clear() {
    context->ClearRenderTargetView(target.Get(), clearColor.elements);
    context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DXVideoAPI::Swap() {
    HRESULT presentResult = swapChain->Present((vsyncEnabled) ? 1 : 0, 0);

    if (presentResult == DXGI_ERROR_DEVICE_REMOVED) {
        CheckDXError(device->GetDeviceRemovedReason());
    } else {
        CheckDXError(presentResult);
    }
}

void DXVideoAPI::SetVsync(bool enabled) {
    vsyncEnabled = enabled;
}

Ref<Shader> DXVideoAPI::CreateShader() {
    return Ref<Shader>(New<DXShader>());
}

Ref<Texture> DXVideoAPI::CreateTexture() {
    return Ref<Texture>(New<DXTexture>());
}

// Does this work cross-compiler?
#pragma push_macro("CreateFont")
#undef CreateFont
Ref<Font> DXVideoAPI::CreateFont() {
    return Ref<Font>();
}
#pragma pop_macro("CreateFont")

Ref<FrameBuffer> DXVideoAPI::CreateFrameBuffer() {
    return Ref<FrameBuffer>(New<DXFrameBuffer>());
}


Ref<ConstantBuffer> DXVideoAPI::CreateConstantBuffer() {
    return Ref<ConstantBuffer>(New<DXConstantBuffer>());
}

void DXVideoAPI::SetTextureRequestHandler(ResourceCatalog& catalog) {
    catalog.SetResourceRequestHandler(ResourceType::Texture, [](Box& box, ResourceType type, StringView name, void* data) {
        DXTexture* tex = New<DXTexture>();
        
        if (!tex->Load(box, name)) {
            CONSOLE_LOG_DEBUG("[{}Resource Error{}] could not load texture {#} from box\n", ConColor::Red, ConColor::White, name);
        }

        return (Resource*)tex;
    });
}

void DXVideoAPI::SetShaderRequestHandler(ResourceCatalog& catalog) {
    catalog.SetResourceRequestHandler(ResourceType::Shader, [](Box& box, ResourceType type, StringView name, void* data) {
        DXShader* shader = New<DXShader>();

        if (!shader->Load(box, name)) {
            CONSOLE_LOG_DEBUG("[{}Resource Error{}] could not load shader {#} from box\n", ConColor::Red, ConColor::White, name);
        }

        return (Resource*)shader;
    });
}

void DXVideoAPI::SetFontRequestHandler(ResourceCatalog& catalog) {
    catalog.SetResourceRequestHandler(ResourceType::Font, [](Box& box, ResourceType type, StringView name, void* data) {
        FTFont* font = New<FTFont>();

        if (!font->Load(box, name)) {
            CONSOLE_LOG_DEBUG("[{}Resource Error{}] could not load font {#} from box\n", ConColor::Red, ConColor::White, name);
        }

        return (Resource*)font;
    });
}

void DXVideoAPI::SetViewport(Vec2i size) {
    D3D11_VIEWPORT viewPort = {};
    viewPort.Width = (float)size.x;
    viewPort.Height = (float)size.y;
    viewPort.MinDepth = 0.0f;
    viewPort.MaxDepth = 1.0f;

    context->RSSetViewports(1, &viewPort);
}

void DXVideoAPI::BindDefaultFrameBuffer() {
    // Unbind all textures to avoid activating frame buffers while that texture is still bound from another draw call
    // Will cause: DEVICE_OMSETRENDERTARGETS_HAZARD
    UnbindAllTextures();
    context->OMSetRenderTargets(1, &target.Get(), depthStencilView.Get());
}

#if !defined(PD_NO_IMGUI)
void DXVideoAPI::ImGuiNewFrame() {
    ImGui_ImplDX11_NewFrame();
}

void DXVideoAPI::ImGuiRender() {
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
}
#endif

ID3D11Device* DXVideoAPI::GetDevice() {
    return device.Get();
}

ID3D11DeviceContext* DXVideoAPI::GetDeviceContext() {
    return context.Get();
}

ID3D11RenderTargetView* DXVideoAPI::GetBackBuffer() {
    return target.Get();
}

void DXVideoAPI::Init() {
    renderer.Reset(New<DXRenderer>());
}

void DXVideoAPI::UnbindAllTextures() {
    ID3D11ShaderResourceView* nullView[1] = { nullptr };
    for (int i = 0; i < PD_MAX_TEXTURE_UNITS; i++) {
        context->PSSetShaderResources(i, 1, nullView);
    }
}

}

#endif
