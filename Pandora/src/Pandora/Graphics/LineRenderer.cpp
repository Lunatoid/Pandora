#include "LineRenderer.h"

#include "Pandora/Graphics/VideoAPI.h"

namespace pd {

void GetLineVertexLayout(DataLayout& layout) {
    layout.Add("position", DataLayoutType::Float3);
    layout.Add("color", DataLayoutType::Float4);
    layout.Finish();
}

LineRenderer::LineRenderer() {
    CreateNewMaterial();

    VideoAPI* video = VideoAPI::Get();
    renderer = video->GetRenderer();
    mvpBuffer = video->CreateConstantBuffer();

    DataLayout mvpLayout;
    mvpLayout.Add("u_mvp", DataLayoutType::Mat4);
    mvpLayout.Finish();

    mvpBuffer->SetLayout(mvpLayout);
    mvpBuffer->SetBindTarget(ShaderType::Vertex);
}

void LineRenderer::UpdateProjection(Mat4 mvp) {
    this->mvp = mvp;
}

void LineRenderer::DrawLine(Vec2 start, Vec2 end, Color color, f32 depth) {
    DrawLine(start, end, color, color, depth);
}

void LineRenderer::DrawLine(Vec2 start, Vec2 end, Color startColor, Color endColor, f32 depth) {
    LineVertex verts[4];

    Vec2 dir = Normalize(end - start);

    if (endCap == LineCap::Box) {
        start -= width * dir;
        end += width * dir;
    }

    Vec2 normal = Vec2(dir.y, -dir.x);
    Vec2 offset = normal * width;

    verts[0].position = start - offset;
    verts[0].position.z = depth;
    verts[0].color = startColor;

    verts[1].position = start + offset;
    verts[1].position.z = depth;
    verts[1].color = startColor;

    verts[2].position = end - offset;
    verts[2].position.z = depth;
    verts[2].color = endColor;

    verts[3].position = end + offset;
    verts[3].position.z = depth;
    verts[3].color = endColor;

    int s = vertices.Count();
    indices.Add(s);
    indices.Add(s + 1);
    indices.Add(s + 3);
    indices.Add(s + 3);
    indices.Add(s + 2);
    indices.Add(s);

    vertices.AddRange(Slice<LineVertex>(verts, 4));

    if (endCap == LineCap::Round) {
        DrawSolidCircle(start, width, startColor, endCapVertices, depth);
        DrawSolidCircle(end, width, endColor, endCapVertices, depth);
    }
}

void LineRenderer::DrawBox(Vec2 start, Vec2 size, Color color, f32 depth) {
    DrawLine(start, start + Vec2(size.x, 0.0f), color, depth);
    DrawLine(start, start + Vec2(0.0f, size.y), color, depth);
    DrawLine(start + Vec2(0.0f, size.y), start + size, color, depth);
    DrawLine(start + Vec2(size.x, 0.0f), start + size, color, depth);
}

void LineRenderer::DrawSolidBox(Vec2 start, Vec2 size, Color color, f32 depth) {
    DrawBox(start, size, color, depth);

    LineVertex verts[4];

    verts[0].position = start;
    verts[0].position.z = depth;
    verts[0].color = color;

    verts[1].position = start + Vec2(size.x, 0.0f);
    verts[1].position.z = depth;
    verts[1].color = color;

    verts[2].position = start + Vec2(0.0f, size.y);
    verts[2].position.z = depth;
    verts[2].color = color;

    verts[3].position = start + size;
    verts[3].position.z = depth;
    verts[3].color = color;

    int s = vertices.Count();
    indices.Add(s);
    indices.Add(s + 1);
    indices.Add(s + 3);
    indices.Add(s + 3);
    indices.Add(s + 2);
    indices.Add(s);

    vertices.AddRange(Slice<LineVertex>(verts, 4));
}

void LineRenderer::DrawCircle(Vec2 center, f32 radius, Color color, int vertexCount, f32 depth) {
    int s = vertices.Count();

    for (int i = 0; i < vertexCount; i++) {
        f32 angle = (f32)i / (f32)vertexCount * PI32 * 2.0f;
        f32 nextAngle = (f32)(i + 1) / (f32)vertexCount * PI32 * 2.0f;

        Vec2 start = Vec2(Cos(angle) * radius + center.x, Sin(angle) * radius + center.y);
        Vec2 end = Vec2(Cos(nextAngle) * radius + center.x, Sin(nextAngle) * radius + center.y);

        DrawLine(start, end, color, depth);
    }
}

void LineRenderer::DrawSolidCircle(Vec2 center, f32 radius, Color color, int vertexCount, f32 depth) {
    int s = vertices.Count();
    vertices.Reserve(vertexCount + 1);
    vertices[s].position = center;
    vertices[s].position.z = depth;
    vertices[s].color = color;

    LineCap end = endCap;
    endCap = LineCap::None;

    for (int i = 1; i < vertexCount + 1; i++) {
        f32 angle = (f32)(i - 1) / (f32)vertexCount * PI32 * 2.0f;

        vertices[s + i].position.x = Cos(angle) * radius + center.x;
        vertices[s + i].position.y = Sin(angle) * radius + center.y;
        vertices[s + i].position.z = depth;
        vertices[s + i].color = color;

        // If we're at the end lastIndex will be `1` to properly wrap around
        u32 lastIndex = (i < vertexCount) ? i + 1 : 1;

        indices.Add(s);
        indices.Add(s + i);
        indices.Add(s + lastIndex);
    }

    endCap = end;
}

void LineRenderer::Render() {
    material->Bind(renderer.Get());

    mvpBuffer->SetElement("u_mvp", mvp);
    mvpBuffer->Upload();

    renderer->DrawIndexed(vertices.Data(), vertices.Count(), indices);

    renderer->Submit();

    vertices.Clear();
    indices.Clear();
}

Material* LineRenderer::GetMaterial() {
    return material.Get();
}

}
