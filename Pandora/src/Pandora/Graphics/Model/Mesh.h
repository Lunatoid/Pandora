#pragma once

#include "Pandora/Core/Math/Vector.h"
#include "Pandora/Core/Data/Array.h"
#include "Pandora/Core/Resources/Resource.h"

#include "Pandora/Graphics/Renderer.h"

namespace pd {

struct MeshVertex {
    Vec3 position;
    Vec3 normal;
    Vec2 uv;
    Vec3 tangent;
};

/**
 * \brief Gets the mesh vertex layout.
 * 
 * \param layout The mesh vertex layout.
 */
void GetMeshVertexLayout(DataLayout& layout);

class Mesh : public Resource {
public:
    static ResourceType GetType() { return ResourceType::Mesh; }

    /**
     * \brief Loads the mesh from a file.
     * 
     * \param path The path to the file.
     * \return Whether or not it loaded successfully.
     */
    virtual bool Load(StringView path) override;

    /**
     * \brief Loads the mesh from a box.
     * 
     * \param box The box.
     * \param name The mesh resource name.
     * \return Whether or not it loaded successfully.
     */
    virtual bool Load(Box& box, StringView name) override;

    /**
     * \return A slice with all the vertices.
     */
    Slice<MeshVertex> GetVertices();

    /**
     * \return A slice with all the indices.
     */
    Slice<u32> GetIndices();

private:
#if !defined(PD_NO_ASSIMP)
    /**
     * \brief Loads the mesh from an Assimp scene.
     * 
     * \param aiScene The Assimp scene.
     */
    void LoadFromScene(const void* aiScene);
#endif

    Array<MeshVertex> vertices;
    Array<u32> indices;
};

}
