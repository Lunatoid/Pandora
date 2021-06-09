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

/**
 * \brief Gets the sprite vertex layout.
 * 
 * \param layout The sprite vertex layout.
 */
void GetSpriteVertexLayout(DataLayout& layout);

class Sprite {
public:
    Sprite() = default;
    Sprite(Sprite& other);
    ~Sprite() = default;

    /**
     * \brief Loads the sprite.
     * 
     * \tparam T The sprite material.
     * \param texName The texture resource name.
     * \param shaderName The shader resource name.
     */
    template<typename T = Material>
    void Load(StringView texName, StringView shaderName = "Shaders/Sprite") {
        CreateNewMaterial<T>(shaderName);

        material->SetTexture(texName, 0);
        size = (Vec2)material->GetTexture(0, RefType::Weak)->GetSize();
    }

    /**
     * \brief Loads the sprite.
     * 
     * \tparam T The sprite material.
     * \param texture The texture.
     * \param shaderName The shader resource name.
     */
    template<typename T = Material>
    void Load(const Ref<Texture>& texture, StringView shaderName = "Shaders/Sprite") {
        CreateNewMaterial<T>(shaderName);

        material->SetTexture(texture, 0);
        size = (Vec2)texture->GetSize();
    }

    /**
     * \brief Generates the sprite vertices.
     * The order is BL > BR > TL > TR.
     * 
     * \param vertices The output vertices.
     */
    void GenerateVertices(SpriteVertex vertices[4]);

    /**
     * \brief Loads a new shader into the current material.
     * 
     * \param shaderName The shader resource name.
     * \param clearTextures Whether or not it should clear all the existing textures.
     */
    void SetMaterial(StringView shaderName, bool clearTextures = false);

    /**
     * \brief Deletes the old material and allocates a new one.
     * 
     * \tparam T The sprite material.
     * \param shaderName The shader resource name.
     */
    template<typename T>
    void CreateNewMaterial(StringView shaderName = "Shaders/Sprite") {
        static_assert(std::is_base_of<Material, T>::value,
                      "Template type T must derive from pd::Material");

        DataLayout layout;
        GetSpriteVertexLayout(layout);
        material.Reset(New<T>());
        material->Load(shaderName, layout);
    }

    /**
     * \return The raw pointer to the material.
     */
    Material* GetMaterial();

    /**
     * \brief Sets the clipping mask of the sprite.
     * Mask values range from 0.0 to `size`.
     * 
     * \param mask The mask.
     */
    void SetClippingMask(Vec4 mask);

    /**
     * \brief Sets the normalized clipping mask.
     * Mask values to range 0.0 and 1.0.
     * 
     * \param mask The mask.
     */
    void SetNormalizedClippingMask(Vec4 mask);

    /**
     * \brief Sets the clipping mask as if it were a tile on an atlas.
     * 
     * \param position The tile position, (0, 0) being the top left.
     * \param tileSize The size of the tile in pixels.
     * \param offset The offset from the top left in pixels.
     */
    void SetAtlasMask(Vec2i position, Vec2 tileSize, Vec2 offset = Vec2(0.0f));

    /**
     * \return The normalized clipping mask.
     */
    Vec4 GetClippingMask();

    /**
     * \brief Returns the size of a texture in the material.
     * 
     * \param slot The slot.
     * \param applyScale Whether or not it should apply the scale.
     * \return The size of the texture.
     */
    Vec2 GetSize(int slot, bool applyScale) const;

    /**
     * \return The hash of the sprite material.
     */
    u64 GetMaterialHash();

    /**
     * \param slot The slot.
     * \return The has of that texture.
     */
    u64 GetTextureHash(int slot = 0);

    /**
     * \return The combined texture hash.
     */
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
