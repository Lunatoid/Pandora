#pragma once

#include "Pandora/Core/Math/Vector.h"
#include "Pandora/Core/Math/Color.h"

#include "Pandora/Graphics/Material.h"

namespace pd {

struct LineVertex {
    Vec3 position;
    Color color;
};

enum class LineCap {
    None,
    Round,
    Box
};

/**
 * \brief Gets the line vertex layout.
 * 
 * \param layout The line vertex layout.
 */
void GetLineVertexLayout(DataLayout& layout);

class LineRenderer {
public:
    LineRenderer();

    /**
     * \brief Creates a new material.
     * 
     * \tparam T The material type or it's derivative.
     * \param shaderName The resource name of the shader to use.
     */
    template<typename T = Material>
    void CreateNewMaterial(StringView shaderName = "Shaders/Line") {
        static_assert(std::is_base_of<Material, T>::value, "Template type T must derive from pd::Material");

        DataLayout layout;
        GetLineVertexLayout(layout);
        material.Reset(New<T>());
        material->Load(shaderName, layout);
    }

    /**
     * \brief Updates the matrix projection.
     * 
     * \param mvp The projection matrix.
     */
    void UpdateProjection(Mat4 mvp);

    // @TODO: 3D lines

    /**
     * \brief Draws a line.
     * 
     * \param start The start position.
     * \param end The end position.
     * \param color The line color.
     * \param depth The line depth.
     */
    void DrawLine(Vec2 start, Vec2 end, Color color, f32 depth);

    /**
     * \brief Draws a line.
     * 
     * \param start The start position.
     * \param end The end position.
     * \param startColor The start color.
     * \param endColor The end color.
     * \param depth The line depth.
     */
    void DrawLine(Vec2 start, Vec2 end, Color startColor, Color endColor, f32 depth);

    /**
     * \brief Draws a box.
     * 
     * \param start The start position.
     * \param size The box size.
     * \param color The box color.
     * \param depth The box depth.
     */
    void DrawBox(Vec2 start, Vec2 size, Color color, f32 depth);

    /**
     * \brief Draws a solid box.
     * 
     * \param start The start position.
     * \param size The box size.
     * \param color The box color.
     * \param depth The box depth.
     */
    void DrawSolidBox(Vec2 start, Vec2 size, Color color, f32 depth);

    /**
     * \brief Draws a circle.
     * 
     * \param center The center position.
     * \param radius The circle radius.
     * \param color The circle color.
     * \param vertexCount How many vertices the circle should have.
     * \param depth The circle depth.
     */
    void DrawCircle(Vec2 center, f32 radius, Color color, int vertexCount, f32 depth);

    /**
     * \brief Draws a solid circle.
     * 
     * \param center The center position.
     * \param radius The circle radius.
     * \param color The circle color.
     * \param vertexCount How many vertices the circle should have.
     * \param depth The circle depth.
     */
    void DrawSolidCircle(Vec2 center, f32 radius, Color color, int vertexCount, f32 depth);

    /**
     * \brief Renders all the lines.
     */
    void Render();

    /**
     * \return A pointer to the material.
     */
    Material* GetMaterial();

    f32 width = 1.0f;
    LineCap endCap = LineCap::Round;
    int endCapVertices = 16;

private:
    Ref<Material> material;
    Ref<Renderer> renderer;
    Ref<ConstantBuffer> mvpBuffer;

    Array<LineVertex> vertices;
    Array<u32> indices;

    Mat4 mvp;
};

}
