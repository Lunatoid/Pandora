#pragma once

#include "Pandora/Core/Types.h"
#include "Pandora/Core/Math/Lerp.h"

namespace pd {

constexpr f64 PI = 3.14159265358979323846;
constexpr f32 PI32 = (f32)PI;

constexpr f64 RAD_TO_DEG = 360.0 / (PI * 2.0);
constexpr f64 DEG_TO_RAD = (PI * 2.0) / 360.0;

/**
 * \param a The first value.
 * \param b The second value.
 * \return The smallest of the two.
 */
template<typename T>
constexpr T Min(const T& a, const T& b) {
    return (a < b) ? a : b;
}

/**
 * \brief 
 * 
 * \param a The first value.
 * \param b The second value.
 * \return The biggest of the two.
 */
template<typename T>
constexpr T Max(const T& a, const T& b) {
    return (a > b) ? a : b;
}

/**
 * \brief Clamps `val` between `min` and `max`.
 * 
 * \param val The value to clamp.
 * \param min The minimum value.
 * \param max The maximum value.
 * \return The clamped value.
 */
template<typename T>
constexpr T Clamp(const T& val, const T& min, const T& max) {
    return Min(max, Max(min, val));
}

/**
 * \param val The value.
 * \return The absolute value.
 */
template<typename T>
constexpr T Abs(const T& val) {
    return (val < T(0)) ? val * T(-1) : val;
}

/**
 * \param val The value.
 * \return The absolute value.
 */
template<typename V>
constexpr V AbsVec(const V& val) {
    V absVal;
    for (int i = 0; i < sizeof(val.elements) / sizeof(val.elements[0]); i++) {
        absVal.elements[i] = Abs(val.elements[i]);
    }

    return absVal;
}

/**
 * \brief Aligns `val` up to a specific alignment.
 * 
 * \param val The value.
 * \param alignment The alignment.
 * \return The aligned value.
 */
template<typename T>
constexpr T AlignUp(const T& val, const T& alignment) {
    return val + (alignment - (val % alignment)) % alignment;
}

/**
 * \brief Converts radians to degrees.
 * 
 * \param radians The value in radians.
 * \return The value in degrees.
 */
f64 ToDegrees(f64 radians);

/**
 * \brief Converts radians to degrees.
 * 
 * \param radians The value in radians.
 * \return The value in degrees.
 */
f32 ToDegrees(f32 radians);

/**
 * \brief Converts degrees to radians.
 * 
 * \param degrees The value in degrees.
 * \return The value in radians.
 */
f64 ToRadians(f64 degrees);

/**
 * \brief Converts degrees to radians.
 * 
 * \param degrees The value in degrees.
 * \return The value in radians.
 */
f32 ToRadians(f32 degrees);

/**
 * \brief How many bytes would be in `kb` kilobytes.
 * 
 * \param kb The kilobyte count.
 * \return The byte count.
 */
f64 Kilobytes(f64 kb);

/**
 * \brief How many bytes would be in `mb` megabytes.
 * 
 * \param mb The megabyte count.
 * \return The byte count.
 */
f64 Megabytes(f64 mb);

/**
 * \brief How many bytes would be in `gb` gigabytes.
 * 
 * \param gb The gigabyte count.
 * \return The byte count.
 */
f64 Gigabytes(f64 gb);

/**
 * \brief Converts bytes to kilobytes.
 * 
 * \param bytes The byte count.
 * \return The kilobyte count.
 */
f64 ToKilobytes(f64 bytes);

/**
 * \brief Converts bytes to megabytes.
 * 
 * \param bytes The byte count.
 * \return The megabyte count.
 */
f64 ToMegabytes(f64 bytes);

/**
 * \brief Converts bytes to gigabytes.
 * 
 * \param bytes The byte count.
 * \return The gigabyte count.
 */
f64 ToGigabytes(f64 bytes);

/**
 * \brief Rounds `v` upwards to the smallest integral value not less than `v`.
 * 
 * \param v The value.
 * \return The rounded value.
 */
f64 Ceil(f64 v);

/**
 * \brief Rounds `v` upwards to the smallest integral value not less than `v`.
 * 
 * \param v The value.
 * \return The rounded value.
 */
f32 Ceil(f32 v);

/**
 * \brief Rounds `v` upwards to the smallest integral value not less than `v`.
 * 
 * \param v The value.
 * \return The rounded value.
 */
int Ceil(int v);

/**
 * \brief Rounds `v` downwards to the biggest integral value not greater than `v`.
 * 
 * \param v The value.
 * \return The rounded value.
 */
f64 Floor(f64 v);

/**
 * \brief Rounds `v` downwards to the biggest integral value not greater than `v`.
 * 
 * \param v The value.
 * \return The rounded value.
 */
f32 Floor(f32 v);

/**
 * \brief Rounds `v` downwards to the biggest integral value not greater than `v`.
 * 
 * \param v The value.
 * \return The rounded value.
 */
int Floor(int v);

/**
 * \brief Rounds `v` to the nearest integral value.
 * 
 * \param v The value.
 * \return The rounded value.
 */
f64 Round(f64 v);

/**
 * \brief Rounds `v` to the nearest integral value.
 * 
 * \param v The value.
 * \return The rounded value.
 */
f32 Round(f32 v);

/**
 * \brief Rounds `v` to the nearest integral value.
 * 
 * \param v The value.
 * \return The rounded value.
 */
int Round(int v);

/**
 * \brief Rounds `v` to the next integral power of 2.
 * 
 * \param v The value.
 * \return The rounded value.
 */
int RoundToPow2(int v);

/**
 * \param v The value.
 * \return Whether or not the value is a power of 2.
 */
bool IsPowerOf2(int v);

/**
 * \param v The value in radians.
 * \return The sine of `v`.
 */
f64 Sin(f64 v);

/**
 * \param v The value in radians.
 * \return The sine of `v`.
 */
f32 Sin(f32 v);

/**
 * \param v The value in radians.
 * \return The cosine of `v`.
 */
f64 Cos(f64 v);

/**
 * \param v The value in radians.
 * \return The cosine of `v`.
 */
f32 Cos(f32 v);

/**
 * \param v The value in radians.
 * \return The tangent of `v`.
 */
f64 Tan(f64 v);

/**
 * \param v The value in radians.
 * \return The tangent of `v`.
 */
f32 Tan(f32 v);

/**
 * \param v The value in radians.
 * \return The arc-sine of `v`.
 */
f64 Asin(f64 v);

/**
 * \param v The value in radians.
 * \return The arc-sine of `v`.
 */
f32 Asin(f32 v);

/**
 * \param v The value in radians.
 * \return The arc-cosine of `v`.
 */
f64 Acos(f64 v);

/**
 * \param v The value in radians.
 * \return The arc-cosine of `v`.
 */
f32 Acos(f32 v);

/**
 * \param v The value in radians.
 * \return The arc-tangent of `v`.
 */
f64 Atan(f64 v);

/**
 * \param v The value in radians.
 * \return The arc-tangent of `v`.
 */
f32 Atan(f32 v);

/**
 * \param y The y value in radians.
 * \param x The x value in radians.
 * \return The arc-tangent of `y/x`.
 */
f64 Atan2(f64 y, f64 x);

/**
 * \param y The y value in radians.
 * \param x The x value in radians.
 * \return The arc-tangent of `y/x`.
 */
f32 Atan2(f32 y, f32 x);

/**
 * \param v The value.
 * \return The natural logarithm of `v`.
 */
f64 Log(f64 v);

/**
 * \param v The value.
 * \return The natural logarithm of `v`.
 */
f32 Log(f32 v);

/**
 * \param v The value.
 * \param base The base.
 * \return The natural logarithm of `v` in the specified base.
 */
f64 Log(f64 v, f64 base);

/**
 * \param v The value.
 * \param base The base.
 * \return The logarithm of `v` in the specified base.
 */
f32 Log(f32 v, f32 base);

/**
 * \param v The value.
 * \return The common logarithm of `v`.
 */
f64 Log10(f64 v);

/**
 * \param v The value.
 * \return The common logarithm of `v`.
 */
f32 Log10(f32 v);

/**
 * \param n The value.
 * \param denom The denominator.
 * \return The modulo.
 */
f64 Mod(f64 n, f64 denom);

/**
 * \param n The value.
 * \param denom The denominator.
 * \return The modulo.
 */
f32 Mod(f32 n, f32 denom);

/**
 * \param v The value.
 * \return +1 if `v` is postive, -1 if `v` is negative.
 */
int Sign(int v);

/**
 * \param v The value.
 * \return +1 if `v` is postive, -1 if `v` is negative.
 */
f64 Sign(f64 v);

/**
 * \param v The value.
 * \return +1 if `v` is postive, -1 if `v` is negative.
 */
f32 Sign(f32 v);

/**
 * \brief Copies the sign of `y` onto `x`.
 * 
 * \param x The value that the sign gets copied to.
 * \param y The sign.
 * \return `x` with the copied sign.
 */
int CopySign(int x, int y);

/**
 * \brief Copies the sign of `y` onto `x`.
 * 
 * \param x The value that the sign gets copied to.
 * \param y The sign.
 * \return `x` with the copied sign.
 */
f32 CopySign(f32 x, f32 y);

/**
 * \brief Copies the sign of `y` onto `x`.
 * 
 * \param x The value that the sign gets copied to.
 * \param y The sign.
 * \return `x` with the copied sign.
 */
f64 CopySign(f64 x, f64 y);

/**
 * \brief Calculates the smallest delta in a looping set.
 * 
 * \param current The current value.
 * \param target The target value.
 * \param boundary The looping boundary.
 * \return The delta between `current` and `target`, looping at 0.0 and `boundary`.
 */
f64 DeltaLooping(f64 current, f64 target, f64 boundary);

/**
 * \brief Calculates the smallest delta in a looping set.
 * 
 * \param current The current value.
 * \param target The target value.
 * \param boundary The looping boundary.
 * \return The delta between `current` and `target`, looping at 0.0 and `boundary`.
 */
f32 DeltaLooping(f32 current, f32 target, f32 boundary);

/**
 * \brief Calculates the smallest delta in a looping set.
 * 
 * \param current The current value.
 * \param target The target value.
 * \param boundary The looping boundary.
 * \return The delta between `current` and `target`, looping at 0.0 and `boundary`.
 */
int DeltaLooping(int current, int target, int boundary);

/**
 * \brief Calculates the smallest delta between two angles.
 * 
 * \param current The current value.
 * \param target The target value.
 * \param boundary The looping boundary.
 * \return The delta between `current` and `target`, looping at 0.0 and 360.0.
 */
f64 DeltaAngle(f64 current, f64 target);

/**
 * \brief Calculates the smallest delta between two angles.
 * 
 * \param current The current value.
 * \param target The target value.
 * \param boundary The looping boundary.
 * \return The delta between `current` and `target`, looping at 0.0 and 360.0.
 */
f32 DeltaAngle(f32 current, f32 target);

/**
 * \brief Moves the value from `current` towards `target`.
 * Makes sure the delta does not exceed `maxDelta`.
 * 
 * \param current The current value.
 * \param target The target value.
 * \param maxDelta The largest delta that can be applied.
 * \return The moved value.
 */
f64 MoveTowards(f64 current, f64 target, f64 maxDelta);

/**
 * \brief Moves the value from `current` towards `target`.
 * Makes sure the delta does not exceed `maxDelta`.
 * 
 * \param current The current value.
 * \param target The target value.
 * \param maxDelta The largest delta that can be applied.
 * \return The moved value.
 */
f32 MoveTowards(f32 current, f32 target, f32 maxDelta);

/**
 * \brief Moves the value from `current` towards `target`.
 * Makes sure the delta does not exceed `maxDelta`.
 * 
 * \param current The current value.
 * \param target The target value.
 * \param maxDelta The largest delta that can be applied.
 * \return The moved value.
 */
int MoveTowards(int current, int target, int maxDelta);

/**
 * \brief Moves the value from `current` towards `target`.
 * Makes sure the delta does not exceed `maxDelta`.
 * Makes sure wrapping between 0.0 and `boundary` is smooth.
 * 
 * \param current The current value.
 * \param target The target value.
 * \param maxDelta The largest delta that can be applied.
 * \param boundary The looping boundary.
 * \return The moved value.
 */
f64 MoveTowardsLooping(f64 current, f64 target, f64 maxDelta, f64 boundary);

/**
 * \brief Moves the value from `current` towards `target`.
 * Makes sure the delta does not exceed `maxDelta`.
 * Makes sure wrapping between 0.0 and `boundary` is smooth.
 * 
 * \param current The current value.
 * \param target The target value.
 * \param maxDelta The largest delta that can be applied.
 * \param boundary The looping boundary.
 * \return The moved value.
 */
f32 MoveTowardsLooping(f32 current, f32 target, f32 maxDelta, f32 boundary);

/**
 * \brief Moves the value from `current` towards `target`.
 * Makes sure the delta does not exceed `maxDelta`.
 * Makes sure wrapping between 0.0 and `boundary` is smooth.
 * 
 * \param current The current value.
 * \param target The target value.
 * \param maxDelta The largest delta that can be applied.
 * \param boundary The looping boundary.
 * \return The moved value.
 */
int MoveTowardsLooping(int current, int target, int maxDelta, int boundary);

/**
 * \brief Moves the value from `current` towards `target`.
 * Makes sure the delta does not exceed `maxDelta`.
 * Makes sure wrapping between 0.0 and 360.0 is smooth.
 * 
 * \param current The current value.
 * \param target The target value.
 * \param maxDelta The largest delta that can be applied.
 * \return The moved value.
 */
f64 MoveTowardsAngle(f64 current, f64 target, f64 maxDelta);

/**
 * \brief Moves the value from `current` towards `target`.
 * Makes sure the delta does not exceed `maxDelta`.
 * Makes sure wrapping between 0.0 and 360.0 is smooth.
 * 
 * \param current The current value.
 * \param target The target value.
 * \param maxDelta The largest delta that can be applied.
 * \return The moved value.
 */
f32 MoveTowardsAngle(f32 current, f32 target, f32 maxDelta);

/**
 * \param power The power.
 * \return The exponent.
 */
f64 Exp(f64 power);

/**
 * \param power The power.
 * \return The exponent.
 */
f32 Exp(f32 power);

/**
 * \param power The power.
 * \return The exponent.
 */
int Exp(int power);

/**
 * \brief Calculates `base` raised to the power of `exp`.
 * 
 * \param base The base.
 * \param exp The exponent.
 * \return The result.
 */
f64 Pow(f64 base, f64 exp);

/**
 * \brief Calculates `base` raised to the power of `exp`.
 * 
 * \param base The base.
 * \param exp The exponent.
 * \return The result.
 */
f32 Pow(f32 base, f32 exp);

/**
 * \brief Calculates `base` raised to the power of `exp`.
 * 
 * \param base The base.
 * \param exp The exponent.
 * \return The result.
 */
int Pow(int base, int exp);

/**
 * \param v The value.
 * \return The square root of `v`.
 */
f64 Sqrt(f64 v);

/**
 * \param v The value.
 * \return The square root of `v`.
 */
f32 Sqrt(f32 v);

/**
 * \param v The value.
 * \return The inverse square root of `v`.
 */
f64 InverseSqrt(f64 v);

/**
 * \param v The value.
 * \return The inverse square root of `v`.
 */
f32 InverseSqrt(f32 v);

/**
 * \param v The value.
 * \return The fast inverse square root of `v`.
 */
f32 FastInverseSqrt(f32 v);

}
