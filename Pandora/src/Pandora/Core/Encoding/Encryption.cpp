#include "Encryption.h"

extern "C" {
#include "Pandora/Libs/tiny-AES/aes.h"
}

#include "Pandora/Core/Math/Math.h"
#include "Pandora/Core/Math/Random.h"

namespace pd {

static_assert(ENCRYPTION_BLOCK_LENGTH == AES_BLOCKLEN && ENCRYPTION_BLOCK_LENGTH == AES_KEYLEN, "The encryption block length is no longer equal to the block/key length!");

// These could be expanded to just a "fill array with random bytes" function

void GenerateRandomIV(Array<byte>& out) {
    int zeroByteCount = 0;
    do {
        zeroByteCount = 0;
        for (int i = 0; i < ENCRYPTION_BLOCK_LENGTH; i++) {
            byte b = (byte)random.Range(0, 255);

            if (b == 0) {
                zeroByteCount += 1;
            }

            out.Add(b);
        }
    } while (zeroByteCount == ENCRYPTION_BLOCK_LENGTH);
}

void GenerateRandomKey(Array<byte>& out) {
    int zeroByteCount = 0;
    do {
        zeroByteCount = 0;
        for (int i = 0; i < ENCRYPTION_BLOCK_LENGTH; i++) {
            byte b = (byte)random.Range(0, 255);

            if (b == 0) {
                zeroByteCount += 1;
            }

            out.Add(b);
        }
    } while (zeroByteCount == ENCRYPTION_BLOCK_LENGTH);
}

void Encrypt(Slice<byte> iv, Slice<byte> key, Slice<byte> data, Array<byte>& out) {
    PD_ASSERT_D(iv.SizeInBytes() == ENCRYPTION_BLOCK_LENGTH, "IV must be %d bytes long, %lld bytes given", ENCRYPTION_BLOCK_LENGTH, iv.SizeInBytes());
    PD_ASSERT_D(key.SizeInBytes() == ENCRYPTION_BLOCK_LENGTH, "key must be %d bytes long", ENCRYPTION_BLOCK_LENGTH);

    out.AddRange(data);
    int originalSize = out.Count();

    int newSize = AlignUp(originalSize, ENCRYPTION_BLOCK_LENGTH);

    if (newSize == originalSize) {
        newSize += ENCRYPTION_BLOCK_LENGTH;
    }

    out.Reserve(newSize - out.Count());

    // Add padding using PKCS7
    byte padByte = byte(newSize - originalSize);
    for (int i = originalSize; i < newSize; i++) {
        out.At(i) = padByte;
    }

    AES_ctx context;
    AES_init_ctx_iv(&context, key.Data(), iv.Data());
    AES_CBC_encrypt_buffer(&context, out.Data(), (u32)out.SizeInBytes());
}

void Decrypt(Slice<byte> iv, Slice<byte> key, Slice<byte> data, Array<byte>& out) {
    PD_ASSERT_D(iv.SizeInBytes() == ENCRYPTION_BLOCK_LENGTH, "IV must be %d bytes long, %lld bytes given", ENCRYPTION_BLOCK_LENGTH, iv.SizeInBytes());
    PD_ASSERT_D(key.SizeInBytes() == ENCRYPTION_BLOCK_LENGTH, "key must be %d bytes long", ENCRYPTION_BLOCK_LENGTH);

    out.AddRange(data);

    PD_ASSERT_D(out.SizeInBytes() % ENCRYPTION_BLOCK_LENGTH == 0, "decryption data must be a multiple of %d", ENCRYPTION_BLOCK_LENGTH);

    AES_ctx context;
    AES_init_ctx_iv(&context, key.Data(), iv.Data());
    AES_CBC_decrypt_buffer(&context, out.Data(), (u32)out.SizeInBytes());

    byte padSize = out.Last();
    out.Resize(out.Count() - (int)padSize);
}

}
