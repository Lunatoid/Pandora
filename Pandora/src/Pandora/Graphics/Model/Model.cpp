#include "Model.h"

namespace pd {

void GetMeshVertexLayout(DataLayout& layout) {
    layout.Add("position", DataLayoutType::Float3);
    layout.Add("normal", DataLayoutType::Float3);
    layout.Add("uv", DataLayoutType::Float2);
    layout.Add("tangent", DataLayoutType::Float3);
    layout.Finish();
}

void Model::SetMaterial(StringView matName, bool clearTextures) {
    material->Load(matName, material->GetLayout());
    if (clearTextures) {
        material->ClearTextures();
    }
}

Material* Model::GetMaterial() {
    return material.Get();
}

Mat4 Model::GetMatrix() {
    // Rotate to make model face forward
    // This should probably be done in a more proper way
    Mat4 mat = Mat4(1.0f);
    mat *= Translate(position);
    mat *= ToMat4(rotation);
    mat *= Scale(scale);
    return mat;
}

Ref<Mesh> Model::GetMesh(RefType type) {
    return mesh.NewRef(type);
}

Slice<MeshVertex> Model::GetVertices() {
    return mesh->GetVertices();
}

Slice<u32> Model::GetIndices() {
    return mesh->GetIndices();
}

u64 Model::GetMaterialHash() {
    return material->GetHash();
}

u64 Model::GetTextureHash(int slot) {
    return material->GetTextureHash(slot);
}

u64 Model::GetTextureHashes() {
    return material->GetTextureHashes();
}

}
