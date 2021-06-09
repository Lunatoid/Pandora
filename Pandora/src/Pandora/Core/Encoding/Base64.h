#pragma once

#include "Pandora/Core/Data/Array.h"
#include "Pandora/Core/Data/String.h"
#include "Pandora/Core/IO/Stream.h"

namespace pd {

// Default settings

const char BASE64_PADDING_CHAR = '=';

const char BASE64_ENCODING_TABLE[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
    'w', 'x', 'y', 'z', '0', '1', '2', '3',
    '4', '5', '6', '7', '8', '9', '+', '/'
};

const int BASE64_DECODING_TABLE[128] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1
};

/**
 * \brief Encodes a sequence of bytes as BASE64.
 * 
 * \param input The input to encode.
 * \param out The output string.
 * \param table The encoding table to use.
 * \param padding The padding character to use.
 */
void Base64Encode(Slice<byte> input,
                  String& out,
                  const char* table = BASE64_ENCODING_TABLE,
                  char padding = BASE64_PADDING_CHAR);

/**
 * \brief Encodes a sequence of bytes as BASE64.
 * 
 * \param input The input to encode.
 * \param out The output stream.
 * \param table The encoding table to use.
 * \param padding The padding character to use.
 */
void Base64Encode(Slice<byte> input,
                  Stream& out,
                  const char* table = BASE64_ENCODING_TABLE,
                  char padding = BASE64_PADDING_CHAR);

/**
 * \brief Decodes a string from BASE64.
 * 
 * \param input The BASE64-encoded string.
 * \param out The output array.
 * \param table The decoding table to use.
 * \param padding The padding character to use.
 */
void Base64Decode(const char* input,
                  Array<byte>& out,
                  const int* table = BASE64_DECODING_TABLE,
                  char padding = BASE64_PADDING_CHAR);

/**
 * \brief Decodes a string from BASE64.
 * 
 * \param input The input string.
 * \param out The output stream.
 * \param table The decoding table to use.
 * \param padding The padding character to use.
 */
void Base64Decode(const char* input,
                  Stream& out,
                  const int* table = BASE64_DECODING_TABLE,
                  char padding = BASE64_PADDING_CHAR);
}
