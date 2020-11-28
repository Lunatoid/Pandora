#pragma once

#include "Pandora/Core/Math/Color.h"
#include "Pandora/Core/Math/VectorInt.h"
#include "Pandora/Core/Data/Reference.h"
#include "Pandora/Core/Window/Window.h"
#include "Pandora/Core/Resources/ResourceCatalog.h"

#include "Pandora/Graphics/Text/Font.h"
#include "Pandora/Graphics/DataLayout.h"
#include "Pandora/Graphics/Shader.h"
#include "Pandora/Graphics/Renderer.h"
#include "Pandora/Graphics/Texture.h"
#include "Pandora/Graphics/FrameBuffer.h"
#include "Pandora/Graphics/VideoBackend.h"

namespace pd {

class ConstantBuffer;

class VideoAPI {
public:
    VideoAPI(Window* window);
    virtual ~VideoAPI() = default;

    /// <summary>
    /// Creates the global video API. This can only be called once.
    /// </summary>
    /// <param name="window">The window.</param>
    /// <param name="backend">The backend.</param>
    /// <returns>A pointer to the global video API.</returns>
    static VideoAPI* Create(Window* window, VideoBackend backend);

    /// <summary>
    /// Returns the global video API.
    /// Will assert in debug mode if no video API has been created.
    /// </summary>
    /// <returns>The global video API.</returns>
    static VideoAPI* Get();

    /// <summary>
    /// Deletes the global video API.
    /// If there is no global video API it will do nothing.
    /// </summary>
    static void Delete();

    /// <summary>
    /// Sets the clear color.
    /// </summary>
    /// <param name="color">The color.</param>
    virtual void SetClearColor(Color color) = 0;

    /// <summary>
    /// Returns the current clear color.
    /// </summary>
    /// <returns>The current clear color.</returns>
    Color GetClearColor() const;

    /// <summary>
    /// Clears the color, depth and stencil buffer.
    /// </summary>
    virtual void Clear() = 0;

    /// <summary>
    /// Swaps the front and backbuffer.
    /// </summary>
    virtual void Swap() = 0;

    /// <summary>
    /// Enables or disabled vsync.
    /// </summary>
    /// <param name="enabled"><c>true</c> for enabled. <c>false</c> for disabled.</param>
    virtual void SetVsync(bool enabled) = 0;

    /// <summary>
    /// Sets the viewport size.
    /// </summary>
    /// <param name="size">The size.</param>
    virtual void SetViewport(Vec2i size) = 0;

    /// <summary>
    /// Unbinds any currently bound frame buffers.
    /// </summary>
    virtual void BindDefaultFrameBuffer() = 0;

    /// <summary>
    /// Returns a reference to the renderer.
    /// </summary>
    /// <returns>A reference to the renderer.</returns>
    Ref<Renderer> GetRenderer();

#if !defined(PD_NO_IMGUI)
    /// <summary>
    /// Starts a new ImGui frame.
    /// </summary>
    virtual void ImGuiNewFrame() = 0;

    /// <summary>
    /// Renders the ImGui frame.
    /// </summary>
    virtual void ImGuiRender() = 0;
#endif

    /// <summary>
    /// Creates a shader.
    /// </summary>
    /// <returns>The shader.</returns>
    virtual Ref<Shader> CreateShader() = 0;

    /// <summary>
    /// Creates a texture.
    /// </summary>
    /// <returns>The texture.</returns>
    virtual Ref<Texture> CreateTexture() = 0;

    /// <summary>
    /// Creates a font.
    /// </summary>
    /// <returns>The font.</returns>
    virtual Ref<Font> CreateFont() = 0;

    /// <summary>
    /// Creates a frame buffer.
    /// </summary>
    /// <returns>The frame buffer.</returns>
    virtual Ref<FrameBuffer> CreateFrameBuffer() = 0;

    /// <summary>
    /// Creates a constant buffer.
    /// </summary>
    /// <returns>The constant buffer.</returns>
    virtual Ref<ConstantBuffer> CreateConstantBuffer() = 0;

    /// <summary>
    /// Installs the backend specific texture loader to the catalog.
    /// </summary>
    /// <param name="catalog">The catalog.</param>
    virtual void SetTextureRequestHandler(ResourceCatalog& catalog) = 0;

    /// <summary>
    /// Installs the backend specific shader loader to the catalog.
    /// </summary>
    /// <param name="catalog">The catalog.</param>
    virtual void SetShaderRequestHandler(ResourceCatalog& catalog) = 0;

    /// <summary>
    /// Installs the backend specific font loader to the catalog.
    /// </summary>
    /// <param name="catalog">The catalog.</param>
    virtual void SetFontRequestHandler(ResourceCatalog& catalog) = 0;

    /// <summary>
    /// Installs the mesh loader to the catalog.
    /// </summary>
    /// <param name="catalog">The catalog.</param>
    virtual void SetMeshRequestHandler(ResourceCatalog& catalog);

    struct {
        TextureFiltering filtering = TextureFiltering::Bilinear;
        TextureWrapping wrapping = TextureWrapping::Clamp;
    } textureOptions;

protected:
    /// <summary>
    /// Gets called after construction.
    /// Use this to initialize variables that rely on <c>VideoAPI::Get()</c>.
    /// </summary>
    virtual void Init() = 0;

    Color clearColor;
    Window* window = nullptr;
    Ref<Renderer> renderer;
};

}
