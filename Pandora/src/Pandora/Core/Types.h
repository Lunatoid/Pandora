#pragma once

#include <stdint.h>

namespace pd {

#define PD_STRINGIFY_(a) #a
#define PD_STRINGIFY(a) PD_STRINGIFY_(a)
#define PD_STRINGIFY_U8_(a) u8#a
#define PD_STRINGIFY_U8(a) PD_STRINGIFY_U8_(a)
#define PD_STRINGIFY_W_(a) L#a
#define PD_STRINGIFY_W(a) PD_STRINGIFY_W_(a)

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef wchar_t wchar;
typedef u8 byte;

//
// UTF-8 stuff
//

/// <summary>
/// Indicates that the address points to a UTF-8 encoded string.
/// </summary>
typedef void uchar;

/// <summary>
/// A Unicode codepoint.
/// </summary>
typedef int codepoint;

/// <summary>
/// Makes the codepoint uppercase if possible.
/// </summary>
/// <param name="point">The codepoint.</param>
/// <returns>The uppercase codepoint.</returns>
codepoint ToUpper(codepoint point);

/// <summary>
/// Makes the codepoint lowercase if possible.
/// </summary>
/// <param name="point">The codepoint.</param>
/// <returns>The lowercase codepoint.</returns>
codepoint ToLower(codepoint point);

/// <summary>
/// Checks if a codepoint is uppercase.
/// </summary>
/// <param name="point">The codepoint.</param>
/// <returns>Whether or not it is uppercase.</returns>
bool IsUpper(codepoint point);

/// <summary>
/// Checks if a codepoint is lowercase.
/// </summary>
/// <param name="point">The codepoint.</param>
/// <returns>Whether or not it is lowercase.</returns>
bool IsLower(codepoint point);

/// <summary>
/// Gets the size of a codepoint in bytes.
/// </summary>
/// <param name="point">The codepoint.</param>
/// <returns>The size in bytes.</returns>
int CodepointSize(codepoint point);

/// <summary>
/// Gets the next codepoint from a UTF-8 string and returns the advanced string.
/// </summary>
/// <param name="string">The UTF-8 string.</param>
/// <param name="out">The codepoint output.</param>
/// <returns>The string after the codepoint.</returns>
uchar* GetNextCodepoint(const uchar* string, codepoint* out);

/// <summary>
/// Returns whether or not the UTF-8 string is valid.
/// </summary>
/// <returns>Whether or not the UTF-8 string is valid.</returns>
bool IsValidUTF8(const uchar* string);

/// <summary>
/// Calculates how many bytes long a UTF-8 string is.
/// </summary>
/// <returns>The length of the string in bytes.</returns>
int UTF8Size(const uchar* string);

/// <summary>
/// Calculates how many codepoints long a UTF-8 string is.
/// </summary>
/// <returns>The length of the string in codepoints.</returns>
int UTF8Count(const uchar* string);



}
