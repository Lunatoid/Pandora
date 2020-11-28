#if !defined(PD_NO_DIRECTX)
#include "DXConstantBuffer.h"

#include "Pandora/Core/IO/Console.h"

#include <d3d11.h>

namespace pd {

DXConstantBuffer::DXConstantBuffer() : video((DXVideoAPI*)VideoAPI::Get()) {
    internalBuffer.Init(DXBufferType::Constant);
}

void DXConstantBuffer::SetBindTarget(ShaderType type) {
    bindTarget = type;
}

void DXConstantBuffer::SetBindSlot(int slot) {
    this->slot = slot;
}

void DXConstantBuffer::SetStruct(Slice<byte> bytes) {
    CheckLayoutSize();

    if (this->bytes.SizeInBytes() != bytes.SizeInBytes()) {
        CONSOLE_LOG_DEBUG("[{}CBuffer Error{}] struct size {} does not equal layout size {}\n",
            ConColor::Red, ConColor::White, bytes.SizeInBytes(), layout.GetStride());
    }

    this->bytes.Clear();
    this->bytes.AddRange(bytes);
}

void DXConstantBuffer::SetElement(StringView name, bool data) {
    CheckLayoutSize();

    int offset = FindElementOffset(name);

    bytes[offset] = (byte)data;
}

void DXConstantBuffer::SetElement(StringView name, f32 data) {
    CheckLayoutSize();
    SetElement(FindElementOffset(name), ToBytes(&data));
}

void DXConstantBuffer::SetElement(StringView name, Vec2 data) {
    CheckLayoutSize();
    SetElement(FindElementOffset(name), ToBytes(&data));
}

void DXConstantBuffer::SetElement(StringView name, Vec3 data) {
    CheckLayoutSize();
    SetElement(FindElementOffset(name), ToBytes(&data));
}

void DXConstantBuffer::SetElement(StringView name, Vec4 data) {
    CheckLayoutSize();
    SetElement(FindElementOffset(name), ToBytes(&data));
}

void DXConstantBuffer::SetElement(StringView name, int data) {
    CheckLayoutSize();
    SetElement(FindElementOffset(name), ToBytes(&data));
}

void DXConstantBuffer::SetElement(StringView name, Vec2i data) {
    CheckLayoutSize();
    SetElement(FindElementOffset(name), ToBytes(&data));
}

void DXConstantBuffer::SetElement(StringView name, Vec3i data) {
    CheckLayoutSize();
    SetElement(FindElementOffset(name), ToBytes(&data));
}

void DXConstantBuffer::SetElement(StringView name, Vec4i data) {
    CheckLayoutSize();
    SetElement(FindElementOffset(name), ToBytes(&data));
}

void DXConstantBuffer::SetElement(StringView name, Mat4 data) {
    CheckLayoutSize();
    SetElement(FindElementOffset(name), ToBytes(&data));
}

void DXConstantBuffer::Upload() {
    if (bytes.SizeInBytes() > 0) {
        internalBuffer.SetData(bytes);

        ID3D11Buffer* buffer = internalBuffer.GetBuffer();
        if (bindTarget == ShaderType::Vertex) {
            video->GetDeviceContext()->VSSetConstantBuffers(slot, 1, &buffer);
        } else {
            video->GetDeviceContext()->PSSetConstantBuffers(slot, 1, &buffer);
        }
    }
}

void DXConstantBuffer::SetElement(int offset, Slice<byte> data) {
    for (int i = 0; i < data.Count(); i++) {
        bytes[offset + i] = data[i];
    }
}

int DXConstantBuffer::FindElementOffset(StringView name) {
    Slice<DataElement> elements = layout.GetElements();

    for (int i = 0; i < elements.Count(); i++) {
        if (name == elements.Data()[i].name) {
            return elements[i].offset;
        }
    }

    CONSOLE_LOG_DEBUG("[{}CBuffer Error{}] could not find element {#} in layout\n", ConColor::Red, ConColor::White, name);
    return 0;
}

void DXConstantBuffer::CheckLayoutSize() {
    // Make sure our bytes array is as big as the stride
    u64 alignedLayoutSize = AlignUp(layout.GetStride(), 8);
    if (bytes.SizeInBytes() != alignedLayoutSize) {
        bytes.Clear();
        bytes.Reserve((int)alignedLayoutSize);
    }
}

}

#endif
