#include "ModelRenderer.h"

#include "Pandora/Graphics/VideoAPI.h"

namespace pd {

ModelRenderer::ModelRenderer() {
    VideoAPI* video = VideoAPI::Get();
    renderer = video->GetRenderer();
    mvpBuffer = video->CreateConstantBuffer();
    
    DataLayout mvpLayout;
    mvpLayout.Add("u_mvp", DataLayoutType::Mat4);
    mvpLayout.Add("u_model", DataLayoutType::Mat4);
    mvpLayout.Finish();

    mvpBuffer->SetLayout(mvpLayout);
    mvpBuffer->SetBindTarget(ShaderType::Vertex);
}

void ModelRenderer::UpdateProjection(Mat4 mvp) {
    this->mvp = mvp;
}

void ModelRenderer::Draw(Model& model) {
    // @TODO: batching like with SpriteRenderer.
    model.GetMaterial()->Bind(renderer.Get());

    mvpBuffer->SetElement("u_mvp", mvp);
    mvpBuffer->SetElement("u_model", model.GetMatrix());
    mvpBuffer->Upload();

    Slice<MeshVertex> vertices = model.GetVertices();
    renderer->DrawIndexed(vertices.Data(), vertices.Count(), model.GetIndices());
    renderer->Submit();
}

}
