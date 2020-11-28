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

/// <summary>
/// Fills the layout with the layout of the <c>MeshVertex</c>.
/// </summary>
/// <param name="layout">The layout.</param>
void GetMeshVertexLayout(DataLayout& layout);

class Mesh : public Resource {
public:
    static ResourceType GetType() { return ResourceType::Mesh; }
    
    /// <summary>
    /// Loads the mesh from a file.
    /// </summary>
    /// <param name="path">The path.</param>
    /// <returns>Whether or not it loaded successfully.</returns>
    virtual bool Load(StringView path) override;
    
    /// <summary>
    /// Loads the mesh from a box.
    /// </summary>
    /// <param name="box">The box.</param>
    /// <param name="name">The resource name.</param>
    /// <returns>Whether or not it loaded successfully.</returns>
    virtual bool Load(Box& box, StringView name) override;
    
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

private:
#if !defined(PD_NO_ASSIMP)
    /// <summary>
    /// Loads the mesh from an Assimp scene.
    /// </summary>
    /// <param name="aiScene">The Assimp scene.</param>
    void LoadFromScene(const void* aiScene);
#endif

    Array<MeshVertex> vertices;
    Array<u32> indices;
};

}
