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

    /**
     * \brief Adds `data` to the vertex data and generates the indices.
     * 
     * \param data A pointer representing 4 vertices.
     */
    virtual void DrawQuad(void* data);

    /**
     * \brief Adds `data` to the vertex and generates indices.
     * 
     * \param data A pointer of multiple quads representing 4 vertices.
     * \param quadCount How many quads are in the `data`.
     */
    virtual void DrawQuads(void* data, int quadCount);

    /**
     * \brief Adds indexed vertices to the vertex data.
     * 
     * \param vertexData The vertex data.
     * \param vertexCount How many vertices are in `vertexData`.
     * \param indexData The indices.
     * \param relativeIndices Whether or not the indices are relative.
     * Relative indices start from 0. Absolute indices can reference existing vertices.
     */
    virtual void DrawIndexed(void* vertexData, int vertexCount, Slice<u32> indexData,
                             bool relativeIndices = true);

    /**
     * \brief Submits the vertex and index data to the GPU.
     * 
     * \param indexOffset The offset from the start.
     * \param indexCount How many indices. Pass -1 for the remaining indices.
     * \param clearData Whether or not to call `ClearData()` after submitting.
     */
    virtual void Submit(int indexOffset = 0, int indexCount = -1, bool clearData = true) = 0;

    /**
     * \brief Sets the active shader.
     * 
     * \param shader The shader.
     */
    void SetShader(const Ref<Shader>& shader);

    /**
     * \return A reference to the active shader.
     */
    Ref<Shader> GetShader();

    /**
     * \brief Binds the active shader.
     */
    void BindShader();

    /**
     * \brief Clears the vertex and index data.
     */
    void ClearData();

protected:

    /**
     * \brief Generates indices for the next `quadCount` quads.
     * 
     * \param quadCount How many quads to generate the indices for.
     */
    void GenerateIndices(int quadCount);

    VideoAPI* video = nullptr;
    Ref<Shader> shader;

    Array<byte> vertices;
    Array<u32> indices;
};

}
