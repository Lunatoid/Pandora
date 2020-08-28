#pragma once

#include "Pandora/Graphics/Model/Mesh.h"

#include "Pandora/Graphics/Material.h"

namespace pd {

class Model {
public:
    /// <summary>
    /// Loads the model.
    /// </summary>
    /// <param name="meshName">Name of the mesh.</param>
    /// <param name="shaderName">Name of the shader.</param>
    void Load(StringView meshName, StringView shaderName = "Shaders/Diffuse");
    
    /// <summary>
    /// Loads the model.
    /// </summary>
    /// <param name="meshName">Reference to the mesh.</param>
    /// <param name="shaderName">Name of the shader.</param>
    void Load(Ref<Mesh> mesh, StringView shaderName = "Shaders/Diffuse");

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
    void CreateNewMaterial(StringView shaderName = "Shaders/Diffuse") {
        static_assert(std::is_base_of<Material, T>::value, "Template type T must derive from pd::Material");

        DataLayout layout;
        GetMeshVertexLayout(layout);
        material.Reset(New<T>());
        material->Load(shaderName, layout);
    }

    /// <summary>
    /// Returns a pointer to the model's material.
    /// </summary>
    /// <returns>A pointer to the model's material.</returns>
    Material* GetMaterial();
    
    /// <summary>
    /// Gets the matrix.
    /// </summary>
    /// <returns>The matrix.</returns>
    Mat4 GetMatrix();
    
    /// <summary>
    /// Gets a reference to the mesh.
    /// </summary>
    /// <param name="type">The reference type.</param>
    /// <returns>The mesh.</returns>
    Ref<Mesh> GetMesh(RefType type = RefType::Strong);
    
    /// <summary>
    /// Gets the vertices.
    /// </summary>
    /// <returns>The vertices.</returns>
    Slice<MeshVertex> GetVertices();

    /// <summary>
    /// Gets the indices.
    /// </summary>
    /// <returns>The indices.</returns>
    Slice<u32> GetIndices();

    /// <summary>
    /// Returns the hash of the model material.
    /// </summary>
    /// <returns>The hash of the model material.</returns>
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
    Vec3 rotation; // @TODO: quaternions
    Color color = Color(1.0f);
    Vec3 scale = Vec3(1.0f);

private:
    Ref<Mesh> mesh;
    Ref<Material> material;
};

}
