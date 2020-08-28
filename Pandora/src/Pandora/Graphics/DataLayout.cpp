#include "DataLayout.h"

namespace pd {

// Size in bytes
const int VertexDataTypeSize[] = {
    0,        // None
    4,        // Float
    4 * 2,    // Float2
    4 * 3,    // Float3
    4 * 4,    // Float4
    4,        // Int
    4 * 2,    // Int2
    4 * 3,    // Int3
    4 * 4,    // Int4
    4 * 4 * 4 // Mat4
};

DataElement::DataElement(StringView name, DataLayoutType type) {
    this->name.Set(name);
    this->type = type;
    size = VertexDataTypeSize[(int)type];
}

DataLayout::DataLayout(bool hlslPacking) : hlslPacking(hlslPacking) {}

DataLayout::~DataLayout() {
    Delete();
}

void DataLayout::Delete() {
    elements.Delete();
}

void DataLayout::Add(StringView name, DataLayoutType type) {
    elements.Add(name, type);
}

void DataLayout::Finish() {
    CalculateOffsetAndStride();
}

int DataLayout::GetStride() const {
    return stride;
}

int DataLayout::Count() const {
    return elements.Count();
}

Slice<DataElement> DataLayout::GetElements() {
    return elements.Slice();
}

void DataLayout::CalculateOffsetAndStride() {
    int offset = 0;

    for (int i = 0; i < elements.Count(); i++) {
        elements[i].offset = offset;

        if (hlslPacking && i + 1 < elements.Count()) {
            DataLayoutType current = elements[i].type;
            DataLayoutType next = elements[i + 1].type;

            // AlignUp to an 8-byte boundary if the current value is a Float3/Int3 and
            // the next value is NOT a Float/Int, this means we can't interleave the
            // next Float/Int as the W component so we need to skip 4 bytes as padding.
            if ((current == DataLayoutType::Float3 && next != DataLayoutType::Float) ||
                (current == DataLayoutType::Int3 && next != DataLayoutType::Int)) {
                // We need to pad
                offset += AlignUp(elements[i].size, 8);
            } else {
                // We can pack as normal
                offset += elements[i].size;
            }

        } else {
            offset += elements[i].size;
        }
    }

    stride = offset;
}

}
