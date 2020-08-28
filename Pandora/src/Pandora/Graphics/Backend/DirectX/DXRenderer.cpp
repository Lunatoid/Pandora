#if !defined(PD_NO_DIRECTX)
#include "DXRenderer.h"

namespace pd {

DXRenderer::DXRenderer() {
    DXVideoAPI* video = (DXVideoAPI*)VideoAPI::Get();
    vertexBuffer.Init(DXBufferType::Vertex);
    indexBuffer.Init(DXBufferType::Index);

    ID3D11RenderTargetView* target = video->GetBackBuffer();
    video->GetDeviceContext()->OMSetRenderTargets(1, &target, nullptr);
}

void DXRenderer::Submit(int indexOffset, int indexCount, bool clearData) {
    if (indexCount <= 0) {
        indexCount = indices.Count() - indexOffset;
    }

    // Set the data
    vertexBuffer.SetData(vertices);
    indexBuffer.SetData(indices.SliceAs<byte>(indexOffset, indexCount));

    // Bind the shaders
    ID3D11DeviceContext* c = ((DXVideoAPI*)VideoAPI::Get())->GetDeviceContext();
    u32 stride = shader->GetLayout().GetStride();

    u32 bufferOffset = 0;
    ID3D11Buffer* vb = vertexBuffer.GetBuffer();
    c->IASetVertexBuffers(0, 1, &vb, &stride, &bufferOffset);

    c->IASetIndexBuffer(indexBuffer.GetBuffer(), DXGI_FORMAT_R32_UINT, 0);

    c->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    c->DrawIndexed(indexCount, 0, 0);

    if (clearData) {
        ClearData();
    }
}

}

#endif
