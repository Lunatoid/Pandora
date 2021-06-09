#include "SpriteRenderer.h"

#include "Pandora/Graphics/VideoAPI.h"

namespace pd {

struct SpriteBatch {
    int offset = 0;
    int quadCount = 0;
    u64 spriteHash = 0;
    Material* material = nullptr;
};

SpriteRenderer::SpriteRenderer() {
    VideoAPI* video = VideoAPI::Get();
    renderer = video->GetRenderer();
    mvpBuffer = video->CreateConstantBuffer();

    DataLayout mvpLayout;
    mvpLayout.Add("u_mvp", DataLayoutType::Mat4);
    mvpLayout.Finish();

    mvpBuffer->SetLayout(mvpLayout);
    mvpBuffer->SetBindTarget(ShaderType::Vertex);
}

void SpriteRenderer::UpdateProjection(Camera cam) {
    this->cam = cam;
}

void SpriteRenderer::Draw(const Sprite& sprite) {
    // Culling!
    if (cam.IsVisible(sprite.position.xy, sprite.GetSize(0, true))) {
        sprites.Add(sprite);
    }
}

void SpriteRenderer::Render() {
    if (sprites.Count() == 0) return;

    sprites.Sort([](const Sprite& a, const Sprite& b) {
        return a.position.z < b.position.z;
    });

    Array<SpriteVertex> vertices;
    vertices.Reserve(sprites.Count() * 4);

    Array<SpriteBatch> batches;
    SpriteBatch currBatch = {};
    currBatch.spriteHash = sprites[0].GetMaterialHash();
    currBatch.material = sprites[0].GetMaterial();

    auto addBatch = [&](int i) {
        batches.Add(currBatch);

        currBatch.offset = i;
        currBatch.spriteHash = sprites[i].GetMaterialHash();
        currBatch.quadCount = 0;
        currBatch.material = sprites[i].GetMaterial();
    };

    for (int i = 0; i < sprites.Count(); i++) {
        if (squishZ) {
            sprites[i].position.z = startSquishDepth + zDistance * (f32)i;
        }

        maxDepth = Max(sprites[i].position.z, maxDepth);

        sprites[i].GenerateVertices(vertices.Data() + (u64)i * 4);

        if (sprites[i].GetMaterialHash() != currBatch.spriteHash) {
            addBatch(i);
        }

        currBatch.quadCount += 1;
    }

    addBatch(sprites.Count() - 1);


    for (int i = 0; i < batches.Count(); i++) {
        batches[i].material->Bind(renderer.Get());
        renderer->DrawQuads(vertices.Data() + (u64)batches[i].offset * 4, batches[i].quadCount);
        mvpBuffer->SetElement("u_mvp", cam.GetMatrix());
        mvpBuffer->Upload();

        renderer->Submit();
    }

    sprites.Clear();
}

void SpriteRenderer::Render(f32 startSquishDepth) {
    this->startSquishDepth = startSquishDepth;
    Render();
    this->startSquishDepth = 0.0f;
}

f32 SpriteRenderer::GetMaxDepth() const {
    return maxDepth;
}

void SpriteRenderer::ResetMaxDepth() {
    maxDepth = 0.0f;
}

}
