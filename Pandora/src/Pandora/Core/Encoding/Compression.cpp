#include "Compression.h"

#include "Pandora/Core/Assert.h"
#include "Pandora/Core/Data/Memory.h"

// @TODO: should these implementations be moved?

#define STBI_ASSERT(x) PD_ASSERT_D(x, u8"Assert from stb_image")
#define STBI_MALLOC(sz) Alloc(sz, pd::Allocator::Persistent)
#define STBI_REALLOC(p, newsz) pd::Realloc(p, newsz, pd::Allocator::Persistent)
#define STBI_FREE(p) pd::Free(p, pd::Allocator::Persistent)
#define STBI_MEMMOVE(a, b, sz) pd::MemoryCopy(a, b, sz, true)
#define STBI_WINDOWS_UTF8
#define STB_IMAGE_IMPLEMENTATION
#include "Pandora/Libs/stb/stb_image.h"

#define STBIW_ASSERT(x) PD_ASSERT_D(x, u8"Assert from stb_image_write")
#define STBIW_MALLOC(sz) pd::Alloc(sz, pd::Allocator::Persistent)
#define STBIW_REALLOC(p, newsz) pd::Realloc(p, newsz, pd::Allocator::Persistent)
#define STBIW_FREE(p) pd::Free(p, pd::Allocator::Persistent)
#define STBIW_MEMMOVE(a, b, sz) pd::MemoryCopy(a, b, sz, true)
#define STBIW_WINDOWS_UTF8
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Pandora/Libs/stb/stb_image_write.h"

namespace pd {

const int COMPRESS_QUALITY = 8;

void CompressData(Slice<byte> bytes, Array<byte>& out) {
    int bufferLen;
    byte* buffer = stbi_zlib_compress(bytes.Data(), bytes.Count(), &bufferLen, COMPRESS_QUALITY);

    out.AddRange(buffer, bufferLen);

    Free(buffer);
}

int CompressData(Slice<byte> bytes, Stream& out) {
    int bufferLen;
    byte* buffer = stbi_zlib_compress(bytes.Data(), bytes.Count(), &bufferLen, COMPRESS_QUALITY);

    int written = out.WriteBytes(Slice<byte>(buffer, bufferLen));

    Free(buffer);

    return written;
}

byte* CompressData(Slice<byte> bytes, int* bufferLength) {
    return stbi_zlib_compress(bytes.Data(), bytes.Count(), bufferLength, COMPRESS_QUALITY);
}

void DecompressData(Slice<byte> bytes, Array<byte>& out) {
    int bufferLen;
    byte* buffer = (byte*)stbi_zlib_decode_malloc((const char*)bytes.Data(), bytes.Count(), &bufferLen);

    out.AddRange(buffer, bufferLen);

    Free(buffer);
}

void DecompressData(Slice<byte> bytes, Stream& out) {
    int bufferLen;
    byte* buffer = (byte*)stbi_zlib_decode_malloc((const char*)bytes.Data(), bytes.Count(), &bufferLen);

    out.WriteBytes(Slice<byte>(buffer, bufferLen));

    Free(buffer);
}

byte* DecompressData(Slice<byte> bytes, int* bufferLength) {
    return (byte*)stbi_zlib_decode_malloc((const char*)bytes.Data(), bytes.Count(), bufferLength);
}

}
