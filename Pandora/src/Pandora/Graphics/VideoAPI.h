#pragma once

#include "Pandora/Core/Math/Color.h"
#include "Pandora/Core/Math/VectorInt.h"
#include "Pandora/Core/Data/Reference.h"
#include "Pandora/Core/Window/Window.h"
#include "Pandora/Core/Resources/ResourceCatalog.h"
#include "Pandora/Core/VideoBackend.h"

#include "Pandora/Graphics/Text/Font.h"
#include "Pandora/Graphics/DataLayout.h"
#include "Pandora/Graphics/Shader.h"
#include "Pandora/Graphics/Renderer.h"
#include "Pandora/Graphics/Texture.h"
#include "Pandora/Graphics/FrameBuffer.h"

namespace pd {

class ConstantBuffer;

class VideoAPI {
public:
    VideoAPI(Window* window);
    virtual ~VideoAPI() = default;

    /**
     * \brief Loads and initializes the video API.
     * 
     * \return Whether or not the backend loaded successfully.
     */
    virtual bool Load() = 0;

    /**
     * \brief Creates the global video API. Can only be called once.
     * 
     * \param window The window with the appropiate backend.
     * \param backend The backend.
     * \return A pointer to the global video API.
     */
    static VideoAPI* Create(Window* window, VideoBackend backend);

    /**
     * \return The global video API.
     */
    static VideoAPI* Get();

    /**
     * \brief Deletes the global video API.
     * If there is none, it does nothing.
     */
    static void Delete();

    /**
     * \brief Sets the clear color.
     * 
     * \param color The color.
     */
    virtual void SetClearColor(Color color) = 0;

    /**
     * \return The clear color.
     */
    Color GetClearColor() const;

    /**
     * \brief Clears the color, depth and stencil buffer.
     */
    virtual void Clear() = 0;

    /**
     * \brief Swaps the front and backbuffer.
     */
    virtual void Swap() = 0;

    /**
     * \brief Enables or disable vsync.
     * 
     * \param enabled Whether it should be enabled or disabled.
     */
    virtual void SetVsync(bool enabled) = 0;

    /**
     * \brief Sets the viewport size.
     * 
     * \param size The size.
     */
    virtual void SetViewport(Vec2i size) = 0;

    /**
     * \brief Unbinds the currently bound framebuffer and binds the default one.
     */
    virtual void BindDefaultFrameBuffer() = 0;

    /**
     * \return A new reference to the renderer.
     */
    Ref<Renderer> GetRenderer();

#if !defined(PD_NO_IMGUI)

    /**
     * \brief Starts a new ImGui frame.
     * 
     */
    virtual void ImGuiNewFrame() = 0;

    /**
     * \brief Renders the ImGui frame.
     */
    virtual void ImGuiRender() = 0;
#endif

    /**
     * \brief Creates a new shader.
     * 
     * \return The new shader.
     */
    virtual Ref<Shader> CreateShader() = 0;

    /**
     * \brief Creates a new texture.
     * 
     * \return The texture.
     */
    virtual Ref<Texture> CreateTexture() = 0;

    /**
     * \brief Creates a new font.
     * 
     * \return The font.
     */
    virtual Ref<Font> CreateFont() = 0;

    /**
     * \brief Creates a new frame buffer.
     * 
     * \return The frame buffer.
     */
    virtual Ref<FrameBuffer> CreateFrameBuffer() = 0;

    /**
     * \brief Creates a new constant buffer.
     * 
     * \return The constant buffer.
     */
    virtual Ref<ConstantBuffer> CreateConstantBuffer() = 0;

    /**
     * \brief Installs the request handler for texture loading.
     * 
     * \param catalog The catalog.
     */
    virtual void SetTextureRequestHandler(ResourceCatalog& catalog) = 0;

    /**
     * \brief Installs the request handler for shader loading.
     * 
     * \param catalog The catalog.
     */
    virtual void SetShaderRequestHandler(ResourceCatalog& catalog) = 0;

    /**
     * \brief Installs the request hander for font loading.
     * 
     * \param catalog The catalog.
     */
    virtual void SetFontRequestHandler(ResourceCatalog& catalog) = 0;

    /**
     * \brief Installs the request handler for mesh loading.
     * 
     * \param catalog The catalog.
     */
    virtual void SetMeshRequestHandler(ResourceCatalog& catalog);

    /**
     * \brief The default import options.
     * Will get overwritten if loading from a box file.
     */
    struct {
        TextureFiltering filtering = TextureFiltering::Bilinear;
        TextureWrapping wrapping = TextureWrapping::Clamp;
    } textureOptions;

protected:

    /**
     * \brief Gets called on construction.
     * Use this to initialize variables that rely on `VideoAPI::Get()`.
     */
    virtual void Init() = 0;

    Color clearColor;
    Window* window = nullptr;
    Ref<Renderer> renderer;
};

}
