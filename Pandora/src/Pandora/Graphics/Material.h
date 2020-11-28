#pragma once

#include "Pandora/Core/Data/Reference.h"
#include "Pandora/Core/Resources/ResourceCatalog.h"

#include "Pandora/Graphics/Shader.h"
#include "Pandora/Graphics/Renderer.h"
#include "Pandora/Graphics/Texture.h"
#include "Pandora/Graphics/ConstantBuffer.h"

namespace pd {

/// <summary>
/// The material base class assumes a simple shader without any constant buffers.
/// Derive from this class and overload e.g. AfterBind() to bind do your own cbuffer shenanigans.
/// </summary>
class Material {
public:
    Material();
    virtual ~Material() = default;
    
    /// <summary>
    /// Loads a material from the specified shader.
    /// </summary>
    /// <param name="shaderName">Name of the shader.</param>
    /// <param name="layout">The layout.</param>
    virtual void Load(StringView shaderName, DataLayout& layout);
    
    /// <summary>
    /// Sets the active shader, binds it and binds all textures.
    /// </summary>
    /// <param name="renderer">The renderer.</param>
    void Bind(Renderer* renderer);
    
    /// <summary>
    /// Sets the texture of the specified slot.
    /// </summary>
    /// <param name="name">The texture name.</param>
    /// <param name="slot">The slot.</param>
    void SetTexture(StringView name, int slot);

    /// <summary>
    /// Sets the texture of the specified slot.
    /// </summary>
    /// <param name="name">The texture name.</param>
    /// <param name="slot">The slot.</param>
    void SetTexture(const Ref<Texture>& texture, int slot);
    
    /// <summary>
    /// Gets a reference of the texture in the specified slot.
    /// </summary>
    /// <param name="slot">The slot.</param>
    /// <param name="type">The type.</param>
    /// <returns>The reference.</returns>
    Ref<Texture> GetTexture(int slot, RefType type = RefType::Strong);
    
    /// <summary>
    /// Returns a slice of all the textures.
    /// </summary>
    /// <returns>The textures.</returns>
    Slice<Ref<Texture>> GetTextures();
    
    /// <summary>
    /// Releases all texture references.
    /// </summary>
    void ClearTextures();
    
    /// <summary>
    /// Returns the layout of the shader.
    /// </summary>
    /// <returns>The layout of the shader.</returns>
    DataLayout& GetLayout();
    
    /// <summary>
    /// Creates a unique hash using the combined texture hash and the shader hash.
    /// </summary>
    /// <returns>The hash.</returns>
    virtual u64 GetHash();
        
    /// <summary>
    /// Returns the hash of the shader,
    /// </summary>
    /// <returns>The hash.</returns>
    u64 GetShaderHash();

    /// <summary>
    /// Returns the hash of the texture in the specified slot.
    /// </summary>
    /// <param name="slot">The slot.</param>
    /// <returns>The hash.</returns>
    u64 GetTextureHash(int slot = 0);
    
    /// <summary>
    /// Creates a hash out of all the textures.
    /// </summary>
    /// <returns>The hash.</returns>
    u64 GetTextureHashes();

    Material& operator=(Material& other) {
        shader = other.shader;

        for (int i = 0; i < textures.Count(); i++) {
            textures[i] = other.textures[i].NewRef(RefType::Strong);
        }

        return *this;
    }

protected:    
    /// <summary>
    /// Creates the combined textures + shader hash.
    /// </summary>
    /// <returns>The hash.</returns>
    u64 TexturesShaderHash();

    /// <summary>
    /// Gets called after the shader has been loaded.
    /// </summary>
    virtual void AfterLoad() {}
    
    /// <summary>
    /// Gets called after a texture has been set.
    /// </summary>
    /// <param name="slot">The slot.</param>
    virtual void AfterSetTexture(int slot) {}

    /// <summary>
    /// Gets called before binding anything.
    /// </summary>
    virtual void BeforeBind() {}
        
    /// <summary>
    /// Gets called after the shader and textures have been bound.
    /// </summary>
    virtual void AfterBind() {}

    ResourceCatalog& catalog;
    Ref<Shader> shader;
    BoundedArray<Ref<Texture>, PD_MAX_TEXTURE_UNITS> textures;
};

}
