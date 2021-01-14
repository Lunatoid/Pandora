#if !defined(PD_NO_DIRECTX)
#include "DXShader.h"

#include "Pandora/Core/IO/File.h"

#include "Pandora/Graphics/Backend/DirectX/DXVideoAPI.h"

namespace pd {

DXShader::DXShader() : video((DXVideoAPI*)VideoAPI::Get()) {}

bool DXShader::Load(StringView name) {
    String vertexPath(Allocator::Temporary);
    String pixelPath(Allocator::Temporary);

    vertexPath.Set(name);
    vertexPath.Append(".Vertex.cso");

    pixelPath.Set(name);
    pixelPath.Append(".Pixel.cso");

    return Load(vertexPath, pixelPath);
}

bool DXShader::Load(Box& box, StringView name) {

    if (box.HasResource(name)) {
        if (box.GetResourceType(name) != ResourceType::Shader) {
            // Type mismatch
            return false;
        }

        Array<byte> data(Allocator::Temporary);
        box.GetResourceData(name, data);

        // Format is as following: <byte, count> <byte, backend> <u32, vertex size> <vertex...> <u32, pixel size> <pixel...>
        MemoryStream memory(data);

        byte backendCount;
        if (memory.ReadByte(&backendCount) != 1) {
            return false;
        }

        if (backendCount == 0) {
            // This shader somehow has no supported backends
            return false;
        }

        u32 vertexSize;
        u32 pixelSize;

        VideoBackend backend = VideoBackend::Count;
        for (int i = 0; i < (int)backendCount; i++) {
            if (memory.Read(&backend) != sizeof(backend)) {
                return false;
            }

            if (backend != VideoBackend::DirectX) {
                // Skip to next backend
                if (memory.Read(&vertexSize) != sizeof(vertexSize)) {
                    return false;
                }

                memory.Seek((i64)vertexSize);

                if (memory.Read(&pixelSize) != sizeof(pixelSize)) {
                    return false;
                }

                memory.Seek((i64)pixelSize);
            } else {
                // Read sources
                if (memory.Read(&vertexSize) != sizeof(vertexSize)) {
                    return false;
                }

                // Get vertex shader
                i64 vertexStart = memory.Position();
                vertexBytecode.AddRange(data.Slice((int)vertexStart, vertexSize));

                memory.Seek((i64)vertexSize);
                if (memory.Read(&pixelSize) != sizeof(pixelSize)) {
                    return false;
                }

                // Get pixel shader
                i64 pixelStart = memory.Position();
                pixelBytecode.AddRange(data.Slice((int)pixelStart, pixelSize));

                CreateFromBytecode();
                return true;
            }
        }

        // We couldn't find a shader for this backend
        return false;
    }

    String vertexPath;
    vertexPath.Set(name);
    vertexPath.Append(".Vertex.cso");
    if (box.HasResource(vertexPath)) {
        if (box.GetResourceType(vertexPath) != ResourceType::Binary) {
            // Type mismatch
            return false;
        }

        box.GetResourceData(vertexPath, vertexBytecode);
    } else {
        return false;
    }

    String pixelPath;
    pixelPath.Set(name);
    pixelPath.Append(".Pixel.cso");
    if (box.HasResource(pixelPath)) {
        if (box.GetResourceType(pixelPath) != ResourceType::Binary) {
            // Type mismatch
            return false;
        }

        box.GetResourceData(pixelPath, pixelBytecode);
    } else {
        return false;
    }

    CreateFromBytecode();
    return true;
}

bool DXShader::Load(StringView vertexPath, StringView pixelPath) {
    ReadEntireFile(vertexPath, vertexBytecode);
    ReadEntireFile(pixelPath, pixelBytecode);

    CreateFromBytecode();
    return true;
}

const DXGI_FORMAT DX_FORMATS[] = {
    DXGI_FORMAT_UNKNOWN,
    DXGI_FORMAT_R32_FLOAT,
    DXGI_FORMAT_R32G32_FLOAT,
    DXGI_FORMAT_R32G32B32_FLOAT,
    DXGI_FORMAT_R32G32B32A32_FLOAT,
    DXGI_FORMAT_R32_SINT,
    DXGI_FORMAT_R32G32_SINT,
    DXGI_FORMAT_R32G32B32_SINT,
    DXGI_FORMAT_R32G32B32A32_SINT
};

void DXShader::SetLayout(DataLayout& layout) {
    PD_ASSERT_D(vertexBytecode.Count() != 0, "No vertex bytecode loaded");

    this->layout.Delete();

    Array<D3D11_INPUT_ELEMENT_DESC> elemDesc(Allocator::Temporary);

    Slice<DataElement> elements = layout.GetElements();
    for (int i = 0; i < elements.Count(); i++) {
        DataElement* e = &elements.Data()[i];

        this->layout.Add(e->name, e->type);

        D3D11_INPUT_ELEMENT_DESC elem = {};
        elem.SemanticName = e->name.CStr();
        elem.SemanticIndex = 0;
        elem.Format = DX_FORMATS[(int)e->type];
        elem.InputSlot = 0;
        elem.AlignedByteOffset = e->offset;
        elem.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
        elem.InstanceDataStepRate = 0;

        elemDesc.Add(elem);
    }

    this->layout.Finish();

    ID3D11Device* d = video->GetDevice();
    ID3D11DeviceContext* c = video->GetDeviceContext();

    CheckDXError(d->CreateInputLayout(elemDesc.Data(), elemDesc.Count(), vertexBytecode.Data(), vertexBytecode.Count(), &vertexLayout.AsOut()));
    c->IASetInputLayout(vertexLayout.Get());
}

DataLayout& DXShader::GetLayout() {
    return layout;
}

void DXShader::Bind() {
    ID3D11DeviceContext* c = video->GetDeviceContext();

    c->VSSetShader(vertexShader.Get(), nullptr, 0);
    c->PSSetShader(pixelShader.Get(), nullptr, 0);

    c->IASetInputLayout(vertexLayout.Get());
}

void DXShader::ActivateTextureSlot(int slot) {
    // Does nothing
}

Slice<byte> DXShader::GetVertexData() {
    return vertexBytecode.Slice();
}

Slice<byte> DXShader::GetPixelData() {
    return pixelBytecode.Slice();
}

void DXShader::CreateFromBytecode() {
    ID3D11Device* d = video->GetDevice();

    // Hash each source and then hash THAT to get our final hash
    u64 hashes[2];

    hashes[0] = DoHash(vertexBytecode.Slice());
    hashes[1] = DoHash(pixelBytecode.Slice());

    hash = DoHash(hashes, sizeof(hashes));

    CheckDXError(d->CreateVertexShader(vertexBytecode.Data(), vertexBytecode.Count(), nullptr, &vertexShader.AsOut()));
    CheckDXError(d->CreatePixelShader(pixelBytecode.Data(), pixelBytecode.Count(), nullptr, &pixelShader.AsOut()));
}

}

#endif
