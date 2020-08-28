#pragma once

#include "Pandora/Core/Data/String.h"
#include "Pandora/Core/Data/Array.h"

namespace pd {

enum class DataLayoutType {
    None,
    Float,
    Float2,
    Float3,
    Float4,
    Int,
    Int2,
    Int3,
    Int4,
    Mat4,
    Count
};

struct DataElement {
    DataElement() = default;
    DataElement(StringView name, DataLayoutType type);

    BoundedString<64> name; // Should be long enough, right?
    DataLayoutType type = DataLayoutType::None;
    int size = 0;
    int offset = 0;
};

class DataLayout {
public:    
    /// <summary>
    /// Initializes the alignment of the layout/
    /// </summary>
    /// <param name="hlslPacking">If <c>true</c> it will abide by HLSL packing rules.</param>
    DataLayout(bool hlslPacking = false);
    ~DataLayout();
    
    /// <summary>
    /// Frees all memory related to this layout. This is called on descruction.
    /// </summary>
    void Delete();

    /// <summary>
    /// Adds an element to the layout.
    /// </summary>
    /// <param name="name">The name.</param>
    /// <param name="type">Type of the data.</param>
    void Add(StringView name, DataLayoutType type);
    
    /// <summary>
    /// Finalizes the layout.
    /// </summary>
    void Finish();
    
    /// <summary>
    /// Returns the stride of the layout.
    /// </summary>
    /// <returns></returns>
    int GetStride() const;
        
    /// <summary>
    /// Returns how many elements are in this layout.
    /// </summary>
    /// <returns>How many elements are in this layout.</returns>
    int Count() const;

    /// <summary>
    /// Returns a slice of all the elements.
    /// </summary>
    /// <returns>A slice of all the elements.</returns>
    Slice<DataElement> GetElements();

private:
    void CalculateOffsetAndStride();

    Array<DataElement> elements;
    int stride = 0;
    bool hlslPacking = false;
};

}
