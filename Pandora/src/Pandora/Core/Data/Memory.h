#pragma once

#include "Pandora/Core/Types.h"

namespace pd {

/// <summary>
/// Attempts to copy <c>copySize</c> bytes of <c>destination</c> to <c>value</c>.
/// </summary>
/// <param name="destination">The destination memory.</param>
/// <param name="destinationSize">How many bytes are left in the memory.</param>
/// <param name="copySize">How many bytes to copy.</param>
/// <param name="useIntermediateBuffer">Whether or not to copy to a temporary memory first.</param>
inline void MemoryCopy(const void* destination, u64 destinationSize, const void* source, u64 copySize, bool useIntermediateBuffer = false);

/// <summary>
/// Copies <c>copySize</c> bytes of <c>destination</c> to <c>value</c>.
/// </summary>
/// <param name="destination">The destination memory.</param>
/// <param name="copySize">How many bytes to copy. Must not be larger than the size of the buffers.</param>
/// <param name="useIntermediateBuffer">Whether or not to copy to a temporary memory first.</param>
void MemoryCopy(const void* destination, const void* source, u64 copySize, bool useIntermediateBuffer = false);

/// <summary>
/// Attempts to set <c>setSize</c> bytes of <c>destination</c> to <c>value</c>.
/// </summary>
/// <param name="destination">The destination memory.</param>
/// <param name="destinationSize">How many bytes are left in the memory.</param>
/// <param name="setSize">How many bytes to set.</param>
/// <param name="value">The byte value to set.</param>
inline void MemorySet(void* destination, u64 destinationSize, u64 setSize, byte value);

/// <summary>
/// Sets <c>setSize</c> bytes of <c>destination</c> to <c>value</c>.
/// </summary>
/// <param name="destination">The destination memory.</param>
/// <param name="setSize">How many bytes to set. Must not be larger than the size of the memory.</param>
/// <param name="value">The byte value to set.</param>
void MemorySet(void* destination, u64 setSize, byte value);

/// <summary>
/// Compares <c>compareSize</c> bytes from <c>a</c> and <c>b</c>.
/// </summary>
/// <param name="a">The first memory.</param>
/// <param name="b">The second memory.</param>
/// <param name="compareSize">How many bytes to compare.</param>
/// <returns><c>true</c> of the block was the same, <c>false</c> if not.</returns>
bool MemoryCompare(void* a, void* b, u64 compareSize);

}
