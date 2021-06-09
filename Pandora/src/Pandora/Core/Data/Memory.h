#pragma once

#include "Pandora/Core/Types.h"

namespace pd {

/**
 * \brief Attempts to copy `copySize` bytes of `destination` to `source`.
 * 
 * \param destination The destination buffer.
 * \param destinationSize The destination buffer size.
 * \param source The source buffer.
 * \param copySize How many bytes to copy.
 * \param useIntermediateBuffer Whether or not to copy to an intermediate buffer first.
 */
inline void MemoryCopy(const void* destination, u64 destinationSize,
                       const void* source, u64 copySize, bool useIntermediateBuffer = false);

/**
 * \brief Copies `copySize` bytes of `destination` to `source`.
 * 
 * \param destination The destination buffer.
 * \param source The source buffer.
 * \param copySize How many bytes to copy.
 * \param useIntermediateBuffer Whether or not to copy to an intermediate buffer first.
 */
void MemoryCopy(const void* destination, const void* source,
                u64 copySize, bool useIntermediateBuffer = false);

/**
 * \brief Attempts to set `setSize` bytes of `destination` to `value`.
 * 
 * \param destination The destination buffer.
 * \param destinationSize The destination buffer size.
 * \param setSize How many bytes to set.
 * \param value The byte value to set.
 */
inline void MemorySet(void* destination, u64 destinationSize, u64 setSize, byte value);

/**
 * \brief Sets `setSize` bytes of `destination` to `value`.
 * 
 * \param destination The destination buffer.
 * \param setSize How many bytes to set.
 * \param value The byte value to set.
 */
void MemorySet(void* destination, u64 setSize, byte value);

/**
 * \brief Compares `compareSize` bytes from `a`  to `b`.
 * 
 * \param a The first buffer.
 * \param b The second buffer.
 * \param compareSize How many bytes to compare.
 * \return True if the buffers were equal, false if otherwise.
 */
bool MemoryCompare(void* a, void* b, u64 compareSize);

}
