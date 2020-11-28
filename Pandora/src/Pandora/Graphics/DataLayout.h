#pragma once

#include "Pandora/Core/Data/String.h"
#include "Pandora/Core/Data/Array.h"

namespace pd {

enum class DataLayoutType : byte {
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

// Size in bytes
const int DataLayoutTypeSize[] = {
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

// Component count
const int DataLayoutComponentCount[] = {
    0, // None
    1, // Float
    2, // Float2
    3, // Float3
    4, // Float4
    1, // Int
    2, // Int2
    3, // Int3
    4, // Int4
    0  // Mat4
};

/// <summary>
/// How data should be packed.
/// `Default`: Normal packing without padding.
/// `Packed`: Packed rules for padding and with packing. Follows the HLSL rules.
/// </summary>
enum class DataPacking : byte {
    Default,
    Packed
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
    DataLayout() = default;
    DataLayout(DataLayout& other);
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
    /// <param name="packing">If <c>true</c> it will abide by Packed packing rules.</param>
    void Finish(DataPacking packing = DataPacking::Default);
    
    /// <summary>
    /// Clears the layout's elements.
    /// </summary>
    void Clear();

    /// <summary>
    /// Returns the stride of the layout.
    /// </summary>
    /// <returns></returns>
    int GetStride() const;

    /// <summary>
    /// Returns the stride of the layout with default packing rules.
    /// </summary>
    /// <returns></returns>
    int GetDefaultStride() const;

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

    DataLayout& operator=(DataLayout& other) {
        CopyFromOther(other);
        return *this;
    }

private:
    int CalculateOffsetAndStride(DataPacking packing);
    void CopyFromOther(DataLayout& other);

    DataPacking lastPacking;
    Array<DataElement> elements;
    int stride = 0;
    int defaultStride = 0;
};

}
