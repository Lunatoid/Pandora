#pragma once

#include "Pandora/Core/Data/Array.h"

namespace pd {

// The length of each block. Also the length of the IV/key.
const int ENCRYPTION_BLOCK_LENGTH = 16;

/// <summary>
/// Generates a random IV. Will never generate an IV that is all zeroes.
/// </summary>
/// <param name="out">Where to store the output.</param>
void GenerateRandomIV(Array<byte>& out);

/// <summary>
/// Generates a random key. Will never generate a key that is all zeroes.
/// </summary>
/// <param name="out">Where to store the output.</param>
void GenerateRandomKey(Array<byte>& out);

/// <summary>
/// Encrypts <c>data</c> using AES/CBC.
/// </summary>
/// <param name="iv">The initialization vector. Must be 16 bytes.</param>
/// <param name="key">The encryption key. Must be 16 bytes.</param>
/// <param name="data">The data to encrypt.</param>
/// <param name="out">Where to store the output.</param>
void Encrypt(Slice<byte> iv, Slice<byte> key, Slice<byte> data, Array<byte>& out);

/// <summary>
/// Decrypts <c>data</c> using AES/CBC.
/// </summary>
/// <param name="iv">The initialization vector. Must be 16 bytes.</param>
/// <param name="key">The encryption key. Must be 16 bytes.</param>
/// <param name="data">The encrypted data.</param>
/// <param name="out">Where to store the output.</param>
void Decrypt(Slice<byte> iv, Slice<byte> key, Slice<byte> data, Array<byte>& out);

}
