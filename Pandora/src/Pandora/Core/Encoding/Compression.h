#pragma once

#include "Pandora/Core/Data/Slice.h"

#include "Pandora/Core/Data/Array.h"
#include "Pandora/Core/IO/Stream.h"

namespace pd {

/**
 * \brief Compresses the input bytes with DEFLATE.
 * 
 * \param bytes The uncompressed input.
 * \param out Where to store the compressed output.
 */
void CompressData(Slice<byte> bytes, Array<byte>& out);

/**
 * \brief Compresses the input bytes with DEFLATE.
 * 
 * \param bytes The uncompressed input.
 * \param out Where to write the compressed output.
 * \return How many bytes were written.
 */
int CompressData(Slice<byte> bytes, Stream& out);

// @TODO: `bufferLength` should probably be a reference and maybe promote it to `u64`.

/**
 * \brief Compresses the input bytes with DEFLATE.
 * Allocates a buffer using the persistent allocator.
 * 
 * \param bytes The uncompressed input.
 * \param bufferLength The output buffer length.
 * \return A pointer to the allocated compressed buffer.
 */
byte* CompressData(Slice<byte> bytes, int* bufferLength);

/**
 * \brief Decompressed the input bytes with DEFLATE.
 * 
 * \param bytes The compressed input.
 * \param out Where to store the uncompressed output.
 */
void DecompressData(Slice<byte> bytes, Array<byte>& out);

/**
 * \brief Decompresses the input bytes with DEFLATE.
 * 
 * \param bytes The compressed input
 * \param out Where to write the decompressed output.
 */
void DecompressData(Slice<byte> bytes, Stream& out);

// @TODO: `bufferLength` should probably be a reference and maybe promote it to `u64`.

/**
 * \brief Decompresses the input bytes with DEFLATE.
 * Allocates a buffer using the persistent allocator.
 * 
 * \param bytes The uncompressed input.
 * \param bufferLength The output buffer length.
 * \return A pointer to the allocated uncompressed buffer.
 */
byte* DecompressData(Slice<byte> bytes, int* bufferLength);

}
