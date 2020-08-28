#include "ConstantBuffer.h"

namespace pd {

void ConstantBuffer::SetLayout(DataLayout& layout) {
    Slice<DataElement> elements = layout.GetElements();

    this->layout.Delete();

    for (int i = 0; i < elements.Count(); i++) {
        this->layout.Add(elements.Data()[i].name, elements[i].type);
    }
    this->layout.Finish();
}

}
