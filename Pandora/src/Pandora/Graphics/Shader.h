#pragma once

#include "Pandora/Core/Data/StringView.h"

#include "Pandora/Core/Resources/Resource.h"

#include "Pandora/Graphics/DataLayout.h"

namespace pd {

enum class ShaderType {
    Vertex,
    Pixel
};

class Shader : public Resource {
public:
    static ResourceType GetType() { return ResourceType::Shader; }

    Shader() = default;
    virtual ~Shader() = default;
    
    /// <summary>
    /// Loads the shader with the specified name.
    /// Will append <c>.Vertex.ext</c> and <c>.Pixel.ext</c> to get the files.
    /// </summary>
    /// <param name="name">The name.</param>
    /// <returns>Whether or not it loaded successfully</returns>
    virtual bool Load(StringView name) = 0;

    /// <summary>
    /// Loads the shader with the specified name from the box.
    /// </summary>
    /// <param name="name">The name.</param>
    /// <returns>Whether or not it loaded successfully</returns>
    virtual bool Load(Box& box, StringView name) = 0;

    /// <summary>
    /// Loads the vertex and pixel shader from the specified path.
    /// </summary>
    /// <param name="vertexSrc">The vertex path.</param>
    /// <param name="pixelSrc">The pixel path.</param>
    /// <returns>Whether or not it loaded successfully</returns>
    virtual bool Load(StringView vertexPath, StringView pixelPath) = 0;
    
    /// <summary>
    /// Sets the shader's vertex layout.
    /// </summary>
    /// <param name="layout">The layout.</param>
    virtual void SetLayout(DataLayout& layout) = 0;
    
    /// <summary>
    /// Returns the shader's current layout.
    /// </summary>
    /// <returns>The shader's current layout.</returns>
    virtual DataLayout& GetLayout() = 0;
    
    /// <summary>
    /// Binds the shader.
    /// </summary>
    virtual void Bind() = 0;
        
    /// <summary>
    /// Activates a texture slot so that it can be used.
    /// </summary>
    /// <param name="slot">The slot.</param>
    virtual void ActivateTextureSlot(int slot) = 0;

    /// <summary>
    /// Returns the data of the platform-specific vertex shader.
    /// </summary>
    /// <returns>The data.</returns>
    virtual Slice<byte> GetVertexData() = 0;

    /// <summary>
    /// Returns the data of the platform-specific vertex shader.
    /// </summary>
    /// <returns>The data.</returns>
    virtual Slice<byte> GetPixelData() = 0;

protected:
    DataLayout layout;
};

}
