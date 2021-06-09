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

    /**
     * \brief Loads the shader from the specified file.
     * Will load `.Vertex.ext` and `.Pixel.ext` to get the files, where `ext` is the backend
     * specific file extension.
     * 
     * \param name The path to the shaders without the extensions.
     * \return Whether or not it loaded successfully.
     */
    virtual bool Load(StringView name) = 0;

    /**
     * \brief Loads the shader with the specific name from the box.
     * 
     * \param box The box.
     * \param name The shader resource name.
     * \return Whether or not it loaded successfully.
     */
    virtual bool Load(Box& box, StringView name) = 0;

    /**
     * \brief Loads the vertex and pixel shader from the specified paths.
     * 
     * \param vertexPath The vertex path.
     * \param pixelPath The pixel path.
     * \return Whether or not it loaded successfully.
     */
    virtual bool Load(StringView vertexPath, StringView pixelPath) = 0;

    /**
     * \brief Sets the layout of the shader.
     * 
     * \param layout The layout.
     */
    virtual void SetLayout(DataLayout& layout) = 0;

    /**
     * \return The current layout of the shader.
     */
    virtual DataLayout& GetLayout() = 0;

    /**
     * \brief Binds the shader.
     */
    virtual void Bind() = 0;

    /**
     * \brief Activates the specified texture slot.
     * 
     * \param slot The slot.
     */
    virtual void ActivateTextureSlot(int slot) = 0;

    /**
     * \return The raw, backend-specific vertex shader data.
     */
    virtual Slice<byte> GetVertexData() = 0;

    /**
     * \return Tge raw, backend-specific pixel data.
     */
    virtual Slice<byte> GetPixelData() = 0;

protected:
    DataLayout layout;
};

}
