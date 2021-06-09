#include "BinaryResource.h"

#include "Pandora/Core/IO/File.h"

namespace pd {

BinaryResource::~BinaryResource() {
    Delete();
}


bool BinaryResource::Load(StringView path) {
    u64 read = ReadEntireFile(path, bytes);

    return read == bytes.SizeInBytes();
}

bool BinaryResource::Load(Box& box, StringView name) {
    if (box.HasResource(name) && box.GetResourceType(name) == ResourceType::Binary) {
        return box.GetResourceData(name, bytes);
    }

    return false;
}

void BinaryResource::Delete() {
    bytes.Delete();
}

Slice<byte> BinaryResource::GetData() {
    return bytes.Slice();
}

}
