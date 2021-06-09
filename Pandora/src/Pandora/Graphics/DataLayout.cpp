#include "DataLayout.h"

namespace pd {

DataElement::DataElement(StringView name, DataLayoutType type) {
    this->name.Set(name);
    this->type = type;
    size = DataLayoutTypeSize[(int)type];
}

DataLayout::DataLayout(DataLayout& other) {
    CopyFromOther(other);
}

DataLayout::~DataLayout() {
    Delete();
}

void DataLayout::Delete() {
    elements.Delete();
}

void DataLayout::Add(StringView name, DataLayoutType type) {
    elements.Add(name, type);
}

void DataLayout::Finish(DataPacking packing) {
    defaultStride = CalculateOffsetAndStride(DataPacking::Default);

    if (packing == DataPacking::Default) {
        stride = defaultStride;
    } else {
        stride = CalculateOffsetAndStride(packing);
    }
}

void DataLayout::Clear() {
    elements.Clear();
}

int DataLayout::GetStride() const {
    return stride;
}

int DataLayout::GetDefaultStride() const {
    return defaultStride;
}

int DataLayout::Count() const {
    return elements.Count();
}

Slice<DataElement> DataLayout::GetElements() {
    return elements.Slice();
}

int DataLayout::CalculateOffsetAndStride(DataPacking packing) {
    int offset = 0;

    auto isAnyVector = [](DataLayoutType type) {
        return (int)type >= (int)DataLayoutType::Float &&
            (int)type <= (int)DataLayoutType::Int4;
    };

    auto getComponentCount = [](DataLayoutType type) {
        return DataLayoutComponentCount[(int)type];
    };

    int currentComponent = 0;
    for (int i = 0; i < elements.Count(); i++) {
        int offsetForCurrent = offset;

        DataLayoutType current = elements[i].type;
        switch (packing) {
            case DataPacking::Default:
                offset += elements[i].size;
                break;

            case DataPacking::Packed:
                if (isAnyVector(current)) {
                    if (currentComponent + getComponentCount(current) > 4) {
                        // Align to nearest 16-byte boundary
                        int difference = 4 * (4 - currentComponent);
                        offsetForCurrent += difference;

                        // This is the new component to count
                        currentComponent = getComponentCount(current);
                        offset += difference + elements[i].size;
                    } else {
                        // We can pack this in the current component
                        currentComponent += getComponentCount(current);
                        offset += elements[i].size;

                        if (currentComponent == 4) {
                            currentComponent = 0;
                        }
                    }
                } else {
                    offset += elements[i].size;
                    currentComponent = 0;
                }
                break;
        }

        elements[i].offset = offsetForCurrent;
    }

    // Align up to nearest 16-byte boundary
    if (packing == DataPacking::Packed && currentComponent != 0) {
        int difference = 4 * (4 - currentComponent);
        offset += difference;
    }

    lastPacking = packing;
    return offset;
}

void DataLayout::CopyFromOther(DataLayout& other) {
    elements.Delete();

    for (int i = 0; i < other.elements.Count(); i++) {
        Add(other.elements[i].name, other.elements[i].type);
    }

    Finish(other.lastPacking);
}

}
