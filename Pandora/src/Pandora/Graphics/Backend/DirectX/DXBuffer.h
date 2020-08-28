#if !defined(PD_NO_DIRECTX)
#pragma once

#include "Pandora/Graphics/Backend/DirectX/DXVideoAPI.h"

struct ID3D11Buffer;

namespace pd {

enum class DXBufferType {
    Vertex,
    Index,
    Constant
};

class DXBuffer {
public:
    ~DXBuffer();

    void Init(DXBufferType type);

    void Resize(int newSize);

    void SetData(Slice<byte> bytes);

    struct ID3D11Buffer* GetBuffer();

private:
    DXVideoAPI* video = nullptr;

    DXBufferType type = DXBufferType::Vertex;
    ID3D11Buffer* buffer = nullptr;

    int size = 0;
};

}

#endif
