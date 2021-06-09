#if !defined(PD_NO_OPENGL)
#pragma once

#include "Pandora/Graphics/VideoAPI.h"

namespace pd {

class GLVideoAPI final : public VideoAPI {
public:
    GLVideoAPI(Window* window);
    virtual ~GLVideoAPI();

    virtual bool Load();

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
    virtual Ref<ConstantBuffer> CreateConstantBuffer() override;
    virtual Ref<FrameBuffer> CreateFrameBuffer() override;

    virtual void SetTextureRequestHandler(ResourceCatalog& catalog) override;
    virtual void SetShaderRequestHandler(ResourceCatalog& catalog) override;
    virtual void SetFontRequestHandler(ResourceCatalog& catalog) override;

protected:
    virtual void Init() override;

private:
    void InitOpenGLExtensions();

    void* nativeData = nullptr;
};

}

#endif
