#pragma once

#include "Pandora/Core/Data/Slice.h"

#include "Pandora/Core/Data/Array.h"
#include "Pandora/Core/IO/Stream.h"

namespace pd {

/// <summary>
/// Compresses data with DEFLATE.
/// </summary>
/// <param name="bytes">The uncompressed input.</param>
/// <param name="out">The destination for the compressed output.</param>
void CompressData(Slice<byte> bytes, Array<byte>& out);

/// <summary>
/// Compresses data with DEFLATE.
/// </summary>
/// <param name="bytes">The uncompressed input.</param>
/// <param name="out">The destination for the compressed output.</param>
/// <returns>How many bytes were written.</returns>
int CompressData(Slice<byte> bytes, Stream* out);

/// <summary>
/// Compresses data with DEFLATE.
/// </summary>
/// <param name="bytes">The uncompressed input.</param>
/// <param name="bufferLength">The output for the length of the buffer.</param>
/// <returns>A pointer to the buffer. Allocated with the persistent allocator.</returns>
byte* CompressData(Slice<byte> bytes, int* bufferLength);

/// <summary>
/// Decompresses data with DEFLATE.
/// </summary>
/// <param name="bytes">The compressed input.</param>
/// <param name="out">The destination for the decompressed output.</param>
void DecompressData(Slice<byte> bytes, Array<byte>& out);

/// <summary>
/// Decompresses data with DEFLATE.
/// </summary>
/// <param name="bytes">The compressed input.</param>
/// <param name="out">The destination for the decompressed output.</param>
void DecompressData(Slice<byte> bytes, Stream* out);

/// <summary>
/// Decompresses data with DEFLATE.
/// </summary>
/// <param name="bytes">The uncompressed output.</param>
/// <param name="bufferLength">The output for the length of the buffer.</param>
/// <returns>A pointer to the buffer. Allocated with the persistent allocator.</returns>
byte* DecompressData(Slice<byte> bytes, int* bufferLength);

}
