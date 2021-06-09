#pragma once

#include "Pandora/Core/Types.h"
#include "Pandora/Core/Math/Math.h"

namespace pd {

/**
 * \tparam maxFlags How many bits can be set.
 */
template<int maxFlags>
class BitMask {
public:
    static_assert(maxFlags > 0, "maxFlags of BitFlag<> must be greater than zero.");
    virtual ~BitMask() {}

    /**
     * \brief Sets the nth bit of the bitfield.
     * 
     * \param index The bit index.
     * \param value The value to set it to.
     */
    void Set(u32 index, bool value);

    /**
     * \brief Gets the nth bit of the bitfield.
     * 
     * \param index The bit index.
     * \return The bit value.
     */
    bool IsSet(u32 index);

private:
    u8 flags[maxFlags / 8] = {};
};

template<int maxFlags>
inline void BitMask<maxFlags>::Set(u32 index, bool value) {
    if (index >= (u32)maxFlags) return;

    int flagIndex = Floor((f32)index / 8.0f);
    int bitIndex = (int)index - (flagIndex * 8);

    if (value) {
        flags[flagIndex] |= 1U << bitIndex;
    } else {
        flags[flagIndex] &= ~(1U << bitIndex);
    }
}

template<int maxFlags>
inline bool BitMask<maxFlags>::IsSet(u32 index) {
    if (index >= (u32)maxFlags) return false;

    int flagIndex = Floor((f32)index / 8.0f);
    int bitIndex = (int)index - (flagIndex * 8);

    return (flags[flagIndex] >> bitIndex) & 1U;
}

}
