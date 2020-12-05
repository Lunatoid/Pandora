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

void GetLineVertexLayout(DataLayout& layout);

class LineRenderer {
public:
    LineRenderer();

    template<typename T = Material>
    void CreateNewMaterial(StringView shaderName = "Shaders/Line") {
        static_assert(std::is_base_of<Material, T>::value, "Template type T must derive from pd::Material");

        DataLayout layout;
        GetLineVertexLayout(layout);
        material.Reset(New<T>());
        material->Load(shaderName, layout);
    }

    void UpdateProjection(Mat4 mvp);

    void DrawLine(Vec2 start, Vec2 end, Color color, f32 depth);
    void DrawLine(Vec2 start, Vec2 end, Color startColor, Color endColor, f32 depth);

    void DrawBox(Vec2 start, Vec2 size, Color color, f32 depth);
    void DrawSolidBox(Vec2 start, Vec2 size, Color color, f32 depth);

    void DrawCircle(Vec2 center, f32 radius, Color color, int vertexCount, f32 depth);
    void DrawSolidCircle(Vec2 center, f32 radius, Color color, int vertexCount, f32 depth);

    void Render();

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
