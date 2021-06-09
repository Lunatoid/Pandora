#pragma once

#include "Pandora/Core/Data/Reference.h"
#include "Pandora/Core/Resources/ResourceCatalog.h"

#include "Pandora/Graphics/Shader.h"
#include "Pandora/Graphics/Renderer.h"
#include "Pandora/Graphics/Texture.h"
#include "Pandora/Graphics/ConstantBuffer.h"

namespace pd {

/**
 * \brief The material base class assumes a simple shader without any constant buffers.
 * Derive from this class and overload e.g. `AfterBind()` to bind and do your custom shenanigans.
 */
class Material {
public:
    Material();
    virtual ~Material() = default;

    /**
     * \brief Loads the shader from the catalog.
     * 
     * \param shaderName The name of the shader resource.
     * \param layout The shader layout.
     */
    virtual void Load(StringView shaderName, DataLayout& layout);

    /**
     * \brief Sets the active shader, binds it and binds all the textures.
     * 
     * \param renderer The renderer.
     */
    void Bind(Renderer* renderer);

    /**
     * \brief Sets the texture of the specified slot.
     * 
     * \param name The texture resource name.
     * \param slot The slot.
     */
    void SetTexture(StringView name, int slot);

    /**
     * \brief Sets the texture of the specified slot.
     * 
     * \param texture The texture.
     * \param slot The slot.
     */
    void SetTexture(const Ref<Texture>& texture, int slot);

    /**
     * \brief Gets the texture from the specified slot.
     * 
     * \param slot The slot.
     * \param type The reference type.
     * \return A new reference to the texture.
     */
    Ref<Texture> GetTexture(int slot, RefType type = RefType::Strong) const;

    /**
     * \return A slice of all the textures.
     */
    Slice<Ref<Texture>> GetTextures();

    /**
     * \brief Releases all texture references.
     */
    void ClearTextures();

    /**
     * \return The layout of the shader.
     */
    DataLayout& GetLayout();

    /**
     * \brief Generates a hash using the combined texture and shader hashes.
     * 
     * \return The hash.
     */
    virtual u64 GetHash();

    /**
     * \return The hash of the shader.
     */
    u64 GetShaderHash();

    /**
     * \param slot The slot.
     * \return Te hash of the texture in that slot.
     */
    u64 GetTextureHash(int slot = 0);

    /**
     * \brief Generates a hash using the combined texture hashes.
     * 
     * \return The hash.
     */
    u64 GetTextureHashes();

    Material& operator=(Material& other) {
        shader = other.shader;

        for (int i = 0; i < textures.Count(); i++) {
            textures[i] = other.textures[i].NewRef(RefType::Strong);
        }

        return *this;
    }

protected:

    /**
     * \brief 
     * 
     * \return Generates the combined texture and shader hash.
     */
    u64 TexturesShaderHash();

    /**
     * \brief Gets called after the shader is loaded.
     * Overload it and do fun stuff!
     */
    virtual void AfterLoad() {}

    /**
     * \brief Gets called after a texture has been set.
     * Overload it and do fun stuff!
     * 
     * \param slot The slot.
     */
    virtual void AfterSetTexture(int slot) {}

    /**
     * \brief Gets called before binding anything in `Bind()`.
     * Overload it and do fun stuff!
     */
    virtual void BeforeBind() {}

    /**
     * \brief Gets called after everything has been bound in `Bind()`.
     * Overload it and do fun stuff!
     */
    virtual void AfterBind() {}

    ResourceCatalog& catalog;
    Ref<Shader> shader;
    BoundedArray<Ref<Texture>, PD_MAX_TEXTURE_UNITS> textures;
};

}
