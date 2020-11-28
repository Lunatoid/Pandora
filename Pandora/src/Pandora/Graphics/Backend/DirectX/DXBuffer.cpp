#if !defined(PD_NO_DIRECTX)
#include "DXBuffer.h"

#include <d3d11.h>

namespace pd {

void DXBuffer::Init(DXBufferType type) {
    this->video = (DXVideoAPI*)VideoAPI::Get();
    this->type = type;
}

void DXBuffer::Resize(int newSize) {
    // The dimensions must be a multiple of 16
    newSize = AlignUp(newSize, 16);

    buffer.Reset();

    const int BIND_BUFFER[] = {
        D3D11_BIND_VERTEX_BUFFER,
        D3D11_BIND_INDEX_BUFFER,
        D3D11_BIND_CONSTANT_BUFFER
    };

    D3D11_BUFFER_DESC desc = {};
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.ByteWidth = (UINT)newSize;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.BindFlags = BIND_BUFFER[(int)type];

    CheckDXError(video->GetDevice()->CreateBuffer(&desc, nullptr, &buffer.AsOut()));

    size = newSize;
}

void DXBuffer::SetData(Slice<byte> bytes) {
    if (size < bytes.Count()) {
        Resize(bytes.Count());
    }

    D3D11_MAPPED_SUBRESOURCE mappedRes = {};
    CheckDXError(video->GetDeviceContext()->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes));

    MemoryCopy(mappedRes.pData, bytes.Data(), bytes.Count());

    video->GetDeviceContext()->Unmap(buffer.Get(), 0);
}

ID3D11Buffer* DXBuffer::GetBuffer() {
    return buffer.Get();
}

}

#endif
