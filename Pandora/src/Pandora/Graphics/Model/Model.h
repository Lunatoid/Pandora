#pragma once

#include "Pandora/Core/Math/Quat.h"

#include "Pandora/Graphics/Material.h"
#include "Pandora/Graphics/Model/Mesh.h"

namespace pd {

class Model {
public:

    /**
     * \brief Loads the model from the catalog.
     * 
     * \tparam T The model material.
     * \param meshName The mesh resource name.
     * \param shaderName The shader resource name.
     */
    template<typename T = Material>
    void Load(StringView meshName, StringView shaderName = "Shaders/Diffuse") {
        CreateNewMaterial<T>(shaderName);
        mesh = ResourceCatalog::Get().Get<Mesh>(meshName);
    }

    /**
     * \brief Loads the model.
     * 
     * \tparam T The model material.
     * \param mesh The mesh.
     * \param shaderName The shader resource name.
     */
    template<typename T = Material>
    void Load(Ref<Mesh> mesh, StringView shaderName = "Shaders/Diffuse") {
        CreateNewMaterial<T>(shaderName);
        this->mesh = mesh;
    }

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
     * \tparam T The model material.
     * \param shaderName The shader resource name.
     */
    template<typename T>
    void CreateNewMaterial(StringView shaderName = "Shaders/Diffuse") {
        static_assert(std::is_base_of<Material, T>::value,
                      "Template type T must derive from pd::Material");

        DataLayout layout;
        GetMeshVertexLayout(layout);
        material.Reset(New<T>());
        material->Load(shaderName, layout);
    }

    /**
     * \return The raw pointer to the material.
     */
    Material* GetMaterial();

    /**
     * \return The model matrix.
     */
    Mat4 GetMatrix();

    /**
     * \param type The reference type.
     * \return A new reference to the mesh.
     */
    Ref<Mesh> GetMesh(RefType type = RefType::Strong);

    /**
     * \return A slice with all mesh vertices.
     */
    Slice<MeshVertex> GetVertices();

    /**
     * \return A slice with all mesh indices.
     */
    Slice<u32> GetIndices();

    /**
     * \return The mateiral hash.
     */
    u64 GetMaterialHash();

    /**
     * \param slot The texture slot.
     * \return The hash of that texture.
     */
    u64 GetTextureHash(int slot = 0);

    /**
     * \return The combined texture hash.
     */
    u64 GetTextureHashes();

    Vec3 position;
    Quat rotation;

    Color color = Color(1.0f);
    Vec3 scale = Vec3(1.0f);

private:
    Ref<Mesh> mesh;
    Ref<Material> material;
};

}
