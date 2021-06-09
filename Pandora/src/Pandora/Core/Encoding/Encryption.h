#pragma once

#include "Pandora/Core/Data/Array.h"

namespace pd {

/**
 * \brief The length of each block. Also the length of the IV and key.
 */
const int ENCRYPTION_BLOCK_LENGTH = 16;

/**
 * \brief Generates a random IV. Will never generate an IV that is all zeroes.
 * 
 * \param out Where to store the output.
 */
void GenerateRandomIV(Array<byte>& out);

/**
 * \brief Generates a random key. Will never generate a key that is all zeroes.
 * 
 * \param out Where to store the output.
 */
void GenerateRandomKey(Array<byte>& out);

/**
 * \brief Encrypts the input data using AES/CBC.
 * 
 * \param iv The initialization vector. Must be 16 bytes.
 * Consider using `GenerateRandomIV()`.
 * \param key The encryption key. Must be 16 bytes.
 * Consider using `GenerateRandomKey()`.
 * \param data The input data to encrypt.
 * \param out Where to store the encrypted output.
 */
void Encrypt(Slice<byte> iv, Slice<byte> key, Slice<byte> data, Array<byte>& out);

/**
 * \brief Decrypts the input data using AES/CBC.
 * 
 * \param iv The initialization vector. Must be 16 bytes.
 * Needs to be the same as the one used with `Encrypt()`.
 * \param key The encryption key. Must be 16 bytes.
 * \param data The input data to decrypt.
 * \param out Where to store the decrypted output.
 */
void Decrypt(Slice<byte> iv, Slice<byte> key, Slice<byte> data, Array<byte>& out);

}
