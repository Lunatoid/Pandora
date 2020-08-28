#pragma once

#include "Pandora/Core/Data/Array.h"
#include "Pandora/Core/Data/Reference.h"

#include "Pandora/Core/Math/Matrix.h"

#include "Pandora/Graphics/Shader.h"

namespace pd {

class VideoAPI;

class Renderer {
public:
    virtual ~Renderer() = default;
    
    /// <summary>
    /// Adds <c>data</c> to the vertex data and generates indices.
    /// </summary>
    /// <param name="data">The vertex data. Interpreted as 4 vertices.</param>
    virtual void DrawQuad(void* data);
    
    /// <summary>
    /// Adds <c>data</c> to the vertex data and generates indices.
    /// </summary>
    /// <param name="data">The vertex data. Interpreted as 4 vertices.</param>
    /// <param name="quadCount">The quad count.</param>
    virtual void DrawQuads(void* data, int quadCount);
        
    /// <summary>
    /// Draws indexed vertex data.
    /// </summary>
    /// <param name="vertexData">The vertex data.</param>
    /// <param name="vertexCount">How many vertices are in <c>data</c>.</param>
    /// <param name="indexData">The index data.</param>
    /// <param name="relativeIndices"><c>true</c> if index data is relative to <c>vertexData</c>. False if it's absolute.</param>
    virtual void DrawIndexed(void* vertexData, int vertexCount, Slice<u32> indexData, bool relativeIndices = true);
        
    /// <summary>
    /// Submits vertex and index data to the GPU.
    /// </summary>
    /// <param name="indexOffset">The index offset.</param>
    /// <param name="indexCount">The index count. -1 for the rest of the indices.</param>
    /// <param name="clearData">Whether or not to call <c>ClearData()</c> after submitting.</param>
    virtual void Submit(int indexOffset = 0, int indexCount = -1, bool clearData = true) = 0;
    
    /// <summary>
    /// Sets the active shader.
    /// </summary>
    /// <param name="shader">The shader.</param>
    void SetShader(Ref<Shader> shader);
        
    /// <summary>
    /// Returns a reference to the shader.
    /// </summary>
    /// <returns>A reference to the shader.</returns>
    Ref<Shader> GetShader();

    /// <summary>
    /// Binds the active shader.
    /// </summary>
    void BindShader();

    /// <summary>
    /// Clears the vertex and index data.
    /// </summary>
    void ClearData();

protected:
    void GenerateIndices(int quadCount);

    VideoAPI* video = nullptr;
    Ref<Shader> shader;

    Array<byte> vertices;
    Array<u32> indices;
};

}
