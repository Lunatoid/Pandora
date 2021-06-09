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

/**
 * \brief A UTF-8 encoded string pointer.
 */
typedef void uchar;

/**
 * \brief A unicode codepoint.
 */
typedef int codepoint;

/**
 * \param point The codepoint.
 * \return The uppercase variant of the codepoint.
 */
codepoint ToUpper(codepoint point);

/**
 * \param point The codepoint.
 * \return The lowercase variant of the codepoint.
 */
codepoint ToLower(codepoint point);

/**
 * \param point The codepoint.
 * \return Whether or not the codepoint is uppercase.
 */
bool IsUpper(codepoint point);

/**
 * \param point The codepoint.
 * \return Whether or not the codepoint is lowercase.
 */
bool IsLower(codepoint point);

/**
 * \param point The codepoint.
 * \return The size of the codepoint in bytes.
 */
int CodepointSize(codepoint point);

/**
 * \brief Gets the next codepoint and advances the string.
 * 
 * \param string The UTF-8 string.
 * \param out The output codepoint.
 * \return The advanced string.
 */
uchar* GetNextCodepoint(const uchar* string, codepoint* out);

/**
 * \param string The UTF-8 string.
 * \return Whether or not the string is valid UTF-8.
 */
bool IsValidUTF8(const uchar* string);

/**
 * \param string The UTF-8 string.
 * \return The size of the string in bytes.
 */
int UTF8Size(const uchar* string);

/**
 * \param string The UTF-8 string.
 * \return The size of the string in codepoints.
 */
int UTF8Count(const uchar* string);



}
