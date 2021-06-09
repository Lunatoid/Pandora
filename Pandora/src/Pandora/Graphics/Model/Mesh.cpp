#include "Mesh.h"

#include "Pandora/Core/IO/Path.h"

#if !defined(PD_NO_ASSIMP)
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#endif

namespace pd {

// Very crude and temporary mesh loading

#if !defined(PD_NO_ASSIMP)
const u32 IMPORT_FLAGS = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices;
#endif

bool Mesh::Load(StringView path) {
#if !defined(PD_NO_ASSIMP)
    const aiScene* scene = aiImportFile(path.CStr(), IMPORT_FLAGS);

    if (!scene) return false;

    LoadFromScene(scene);
    return true;
#else
    PD_ASSERT_D(false, "Assimp not included. Cannot load meshes.");
    return false;
#endif
}

bool Mesh::Load(Box& box, StringView name) {
    if (!box.HasResource(name)) return false;

    ResourceType type = box.GetResourceType(name);

    switch (type) {
        case ResourceType::Binary: {
#if !defined(PD_NO_ASSIMP)
            Array<byte> data(Allocator::Temporary);
            box.GetResourceData(name, data);

            const aiScene* scene = aiImportFileFromMemory((char*)data.Data(), data.Count(),
                                                          IMPORT_FLAGS, nullptr);

            if (!scene) return false;

            LoadFromScene(scene);
            return true;
#else
            PD_ASSERT_D(false, "Assimp not included. Cannot load meshes.");
            return false;
#endif
        }

        case ResourceType::Mesh: {
            Array<byte> data;
            box.GetResourceData(name, data);

            // Mesh format is <u32, vertex count> <u32, index count> <MeshVertex, ...> <u32, ...>
            MemoryStream memory(data);
            u32 vertexCount;
            if (memory.Read(&vertexCount) != sizeof(vertexCount)) {
                return false;
            }
            u32 indexCount;
            if (memory.Read(&indexCount) != sizeof(indexCount)) {
                return false;
            }

            vertices.AddRange(data.SliceAs<MeshVertex>(sizeof(u32) * 2, vertexCount * sizeof(MeshVertex)));

            int indicesOffset = sizeof(u32) * 2 + vertexCount * sizeof(MeshVertex);
            indices.AddRange(data.SliceAs<u32>(indicesOffset, indexCount * sizeof(u32)));
            break;
        }

        default:
            return false;
    }

    return true;
}

Slice<MeshVertex> Mesh::GetVertices() {
    return vertices;
}

Slice<u32> Mesh::GetIndices() {
    return indices;
}

#if !defined(PD_NO_ASSIMP)
void Mesh::LoadFromScene(const void* aiScene) {
    const ::aiScene* scene = (const ::aiScene*)aiScene;

    aiMesh* mesh = scene->mMeshes[0];

    for (u32 i = 0; i < mesh->mNumVertices; i++) {
        MeshVertex v;
        v.position = Vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);

        if (mesh->mTextureCoords) {
            v.uv = Vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }

        if (mesh->mNormals) {
            v.normal = Vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        }

        vertices.Add(v);
    }

    for (u32 i = 0; i < mesh->mNumFaces; i++) {
        for (u32 j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
            indices.Add(mesh->mFaces[i].mIndices[j]);
        }
    }

    aiReleaseImport(scene);
}
#endif

}
