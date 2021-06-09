#include "ModelRenderer.h"

#include "Pandora/Graphics/VideoAPI.h"

namespace pd {

ModelRenderer::ModelRenderer() {
    VideoAPI* video = VideoAPI::Get();
    renderer = video->GetRenderer();
    mvpBuffer = video->CreateConstantBuffer();

    DataLayout mvpLayout;
    mvpLayout.Add("u_view_proj", DataLayoutType::Mat4);
    mvpLayout.Add("u_model", DataLayoutType::Mat4);
    mvpLayout.Finish(DataPacking::Packed);

    mvpBuffer->SetLayout(mvpLayout);
    mvpBuffer->SetBindTarget(ShaderType::Vertex);
}

void ModelRenderer::UpdateProjection(Mat4 viewProj) {
    this->viewProj = viewProj;
}

void ModelRenderer::Draw(Model& model) {
    // @TODO: batching like with SpriteRenderer.
    model.GetMaterial()->Bind(renderer.Get());

    mvpBuffer->SetElement("u_view_proj", viewProj);
    mvpBuffer->SetElement("u_model", model.GetMatrix());
    mvpBuffer->Upload();

    Slice<MeshVertex> vertices = model.GetVertices();
    renderer->DrawIndexed(vertices.Data(), vertices.Count(), model.GetIndices());
    renderer->Submit();
}

}
