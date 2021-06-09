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

/**
 * \brief `Default`: normal packing without padding.
 * `Packed`: Packed rules for padding. Follows the HLSL rules.
 */
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

    /**
     * \brief Frees all memory related to the layout. Gets called on destruction.
     */
    void Delete();

    /**
     * \brief Adds an element to the layout.
     * 
     * \param name The name of the field.
     * \param type The datatype.
     */
    void Add(StringView name, DataLayoutType type);

    /**
     * \brief Finalizes the layout and calculates the stride.
     * 
     * \param packing What packing rules to follow.
     */
    void Finish(DataPacking packing = DataPacking::Default);

    /**
     * \brief Clears the layout's elements.
     */
    void Clear();

    /**
     * \return The stride of the layout.
     */
    int GetStride() const;

    /**
     * \return The stride with the default packing rules.
     */
    int GetDefaultStride() const;

    /**
     * \return How many elements are in this layout.
     */
    int Count() const;

    /**
     * \return A slice of all the elements.
     */
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
