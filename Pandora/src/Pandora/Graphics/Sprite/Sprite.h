#pragma once

#include "Pandora/Core/Math/Vector.h"
#include "Pandora/Core/Math/Color.h"

#include "Pandora/Graphics/Material.h"

namespace pd {

struct SpriteVertex {
    Vec3 vertPos;
    Vec3 position;
    Color color;
    Vec2 uv;
    Vec2 pivot;
    f32 rotation;
    Vec2 size;
};

/// <summary>
/// Fills the layout with the layout of the <c>SpriteVertex</c>.
/// </summary>
/// <param name="layout">The layout.</param>
void GetSpriteVertexLayout(DataLayout& layout);

class Sprite {
public:
    Sprite() = default;
    Sprite(Sprite& other);
    
    /// <summary>
    /// Loads the sprite.
    /// </summary>
    /// <param name="texName">Name of the texture.</param>
    /// <param name="shaderName">Name of the shader.</param>
    void Load(StringView texName, StringView shaderName = "Shaders/Sprite");
        
    /// <summary>
    /// Loads the sprite.
    /// </summary>
    /// <param name="texture">Reference to the texture.</param>
    /// <param name="shaderName">Name of the shader.</param>
    void Load(Ref<Texture> texture, StringView shaderName = "Shaders/Sprite");
    
    /// <summary>
    /// Generates the sprite vertices.
    /// Order is BL>BR>TL>TR.
    /// </summary>
    /// <param name="vertices">Where to store the vertices.</param>
    void GenerateVertices(SpriteVertex vertices[4]);
    
    /// <summary>
    /// Loads a new shader into the current material.
    /// </summary>
    /// <param name="shaderName">Name of the shader.</param>
    /// <param name="clearTextures">If set to <c>true</c> it will clear all the texture slots of this material.</param>
    void SetMaterial(StringView shaderName, bool clearTextures = false);

    /// <summary>
    /// Deletes the old material and allocates a new material from a derived class.
    /// </summary>
    /// <param name="shaderName">Name of the shader.</param>
    template<typename T>
    void CreateNewMaterial(StringView shaderName = "Shaders/Sprite") {
        static_assert(std::is_base_of<Material, T>::value, "Template type T must derive from pd::Material");
        
        DataLayout layout;
        GetSpriteVertexLayout(layout);
        material.Reset(New<T>());
        material->Load(shaderName, layout);
    }
    
    /// <summary>
    /// Returns a pointer to the sprite's material.
    /// </summary>
    /// <returns>A pointer to the sprite's material.</returns>
    Material* GetMaterial();
    
    /// <summary>
    /// Sets the clipping mask of the sprite.
    /// Mask values range from 0 to <c>size</c>.
    /// </summary>
    /// <param name="mask">The mask.</param>
    void SetClippingMask(Vec4 mask);

    /// <summary>
    /// Sets the normalized clipping mask of the sprite.
    /// Mask values range from 0 to 1.
    /// </summary>
    /// <param name="mask">The mask.</param>
    void SetNormalizedClippingMask(Vec4 mask);
    
    /// <summary>
    /// Returns the normalized clipping mask of the sprite.
    /// </summary>
    /// <returns>The normalized clipping mask.</returns>
    Vec4 GetClippingMask();
    
    /// <summary>
    /// Returns the size of a texture in the material.
    /// </summary>
    /// <param name="slot">The slot.</param>
    /// <param name="applyScale">If <c>true</c> it will apply the scale of the sprite.</param>
    /// <returns>The size of the texture.</returns>
    Vec2 GetSize(int slot, bool applyScale);
    
    /// <summary>
    /// Returns the hash of the sprite material.
    /// </summary>
    /// <returns>The hash of the sprite material.</returns>
    u64 GetMaterialHash();
    
    /// <summary>
    /// Returns the hash of a texture in the material.
    /// </summary>
    /// <param name="slot">The slot.</param>
    /// <returns>The hash.</returns>
    u64 GetTextureHash(int slot = 0);
    
    /// <summary>
    /// Returns the hash of all the textures in the material.
    /// </summary>
    /// <returns>The hash.</returns>
    u64 GetTextureHashes();

    Vec3 position;
    Color color = Color(1.0f);
    Vec2 size;
    Vec2 scale = Vec2(1.0f);
    Vec2 pivot;
    f32 rotation = 0.0f;

private:
    Ref<Material> material;
    Vec4 uv = Vec4(0.0f, 0.0f, 1.0f, 1.0f);
};

}
