#include "Renderer.h"

#include "Pandora/Graphics/VideoAPI.h"

namespace pd {

void Renderer::DrawQuad(void* data) {
    DrawQuads(data, 1);
}

void Renderer::DrawQuads(void* data, int quadCount) {
    GenerateIndices(quadCount);

    int vertexCountInBytes = shader->GetLayout().GetStride() * 4 * quadCount;
    vertices.AddRange((byte*)data, vertexCountInBytes);
}

void Renderer::DrawIndexed(void* vertexData, int vertexCount, Slice<u32> indexData, bool relativeIndices) {

    int vertexSize = shader->GetLayout().GetDefaultStride();

    if (relativeIndices && indices.Count() > 0) {
        Array<u32> absIndices(Allocator::Temporary);
        absIndices.AddRange(indexData);
    
        int offset = vertices.Count() / vertexSize;
        for (int i = 0; i < absIndices.Count(); i++) {
            absIndices[i] += offset;
        }

        indices.AddRange(absIndices);
    } else {
        indices.AddRange(indexData);
    }

    vertices.AddRange((byte*)vertexData, vertexCount * vertexSize);
}

void Renderer::SetShader(const Ref<Shader>& shader) {
    this->shader = shader;
}

Ref<Shader> Renderer::GetShader() {
    return shader;
}

void Renderer::BindShader() {
    shader->Bind();
}

void Renderer::ClearData() {
    vertices.Clear();
    indices.Clear();
}

void Renderer::GenerateIndices(int quadCount) {
    int currentQuads = vertices.Count() / shader->GetLayout().GetStride();

    for (int i = 0; i < quadCount; i++) {
        int offset = i * 4;
        indices.Add(currentQuads + offset + 0);
        indices.Add(currentQuads + offset + 1);
        indices.Add(currentQuads + offset + 3);
        indices.Add(currentQuads + offset + 3);
        indices.Add(currentQuads + offset + 2);
        indices.Add(currentQuads + offset + 0);
    }
}

}
