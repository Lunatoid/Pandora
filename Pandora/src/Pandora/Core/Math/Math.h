#pragma once

#include "Pandora/Core/Types.h"
#include "Pandora/Core/Math/Lerp.h"

namespace pd {

constexpr f64 PI = 3.14159265358979323846;
constexpr f32 PI32 = (f32)PI;

constexpr f64 RAD_TO_DEG = 360.0 / (PI * 2.0);
constexpr f64 DEG_TO_RAD = (PI * 2.0) / 360.0;

/// <summary>
/// Returns the minimum of two values.
/// </summary>
/// <param name="a">The first value.</param>
/// <param name="b">The seconds value.</param>
/// <returns>The minimum.</returns>
template<typename T>
constexpr T Min(const T& a, const T& b) {
    return (a < b) ? a : b;
}

/// <summary>
/// Returns the maximum of two values.
/// </summary>
/// <param name="a">The first value.</param>
/// <param name="b">The seconds value.</param>
/// <returns>The maximum.</returns>
template<typename T>
constexpr T Max(const T& a, const T& b) {
    return (a > b) ? a : b;
}

/// <summary>
/// Clamps <c>val</c> between <c>min</c> and <c>max</c>.
/// </summary>
/// <param name="val">The value to clamp.</param>
/// <param name="min">The minimum value.</param>
/// <param name="max">The maximum value.</param>
/// <returns>The clamped value.</returns>
template<typename T>
constexpr T Clamp(const T& val, const T& min, const T& max) {
    return Min(max, Max(min, val));
}

/// <summary>
/// </summary>
/// <param name="val">The value.</param>
/// <returns>The absolute value.</returns>
template<typename T>
constexpr T Abs(const T& val) {
    return (val < T(0)) ? val * T(-1) : val;
}

/// <summary>
/// Aligns a value up to a specific alignment.
/// </summary>
/// <param name="val">The value.</param>
/// <param name="alignment">The alignment.</param>
/// <returns>The aligned value.</returns>
template<typename T>
constexpr T AlignUp(const T& val, const T& alignment) {
    return val + (alignment - (val % alignment)) % alignment;
}

/// <summary>
/// Converts radians to degrees.
/// </summary>
/// <param name="radians">The radians.</param>
/// <returns>The degrees.</returns>
f64 ToDegrees(f64 radians);

/// <summary>
/// Converts radians to degrees.
/// </summary>
/// <param name="radians">The radians.</param>
/// <returns>The degrees.</returns>
f32 ToDegrees(f32 radians);

/// <summary>
/// Converts degrees to radians.
/// </summary>
/// <param name="radians">The degrees.</param>
/// <returns>The radians.</returns>
f64 ToRadians(f64 degrees);

/// <summary>
/// Converts degrees to radians.
/// </summary>
/// <param name="radians">The degrees.</param>
/// <returns>The radians.</returns>
f32 ToRadians(f32 degrees);

/// <summary>
/// Returns how many bytes <c>kb</c> kilobytes is.
/// </summary>
/// <param name="kb">The kilobyte amount.</param>
/// <returns>As bytes.</returns>
f64 Kilobytes(f64 kb);

/// <summary>
/// Returns how many bytes <c>mb</c> megabytes is.
/// </summary>
/// <param name="mb">The megabyte amount.</param>
/// <returns>As bytes.</returns>
f64 Megabytes(f64 mb);

/// <summary>
/// Returns how many bytes <c>gb</c> gigabytes is.
/// </summary>
/// <param name="gb">The gigabyte amount.</param>
/// <returns>As bytes.</returns>
f64 Gigabytes(f64 gb);

/// <summary>
/// Converts <c>bytes</c> to kilobytes.
/// </summary>
/// <param name="bytes">The bytes.</param>
/// <returns>The kilobytes.</returns>
f64 ToKilobytes(f64 bytes);

/// <summary>
/// Converts <c>bytes</c> to megabytes.
/// </summary>
/// <param name="bytes">The bytes.</param>
/// <returns>The megabytes.</returns>
f64 ToMegabytes(f64 bytes);

/// <summary>
/// Converts <c>bytes</c> to gigabytes.
/// </summary>
/// <param name="bytes">The bytes.</param>
/// <returns>The gigabytes.</returns>
f64 ToGigabytes(f64 bytes);

/// <summary>
/// Computes the smallest integer value not less than <c>v</c>.
/// </summary>
/// <param name="v">The value.</param>
/// <returns>The smallest integer value not less than <c>v</c>.</returns>
f64 Ceil(f64 v);

/// <summary>
/// Computes the smallest integer value not less than <c>v</c>.
/// </summary>
/// <param name="v">The value.</param>
/// <returns>The smallest integer value not less than <c>v</c>.</returns>
f32 Ceil(f32 v);

/// <summary>
/// Computes the smallest integer value not less than <c>v</c>.
/// </summary>
/// <param name="v">The value.</param>
/// <returns>The smallest integer value not less than <c>v</c>.</returns>
int Ceil(int v);

/// <summary>
/// Computes the largest integer value not greater than <c>v</c>.
/// </summary>
/// <param name="v">The value.</param>
/// <returns>The largest integer value not greater than <c>v</c>.</returns>
f64 Floor(f64 v);

/// <summary>
/// Computes the largest integer value not greater than <c>v</c>.
/// </summary>
/// <param name="v">The value.</param>
/// <returns>The largest integer value not greater than <c>v</c>.</returns>
f32 Floor(f32 v);

/// <summary>
/// Computes the largest integer value not greater than <c>v</c>.
/// </summary>
/// <param name="v">The value.</param>
/// <returns>The largest integer value not greater than <c>v</c>.</returns>
int Floor(int v);

/// <summary>
/// Computes the nearest integer value to <c>v</c>.
/// </summary>
/// <param name="v"> The value.</param>
/// <returns>The nearest integer value to <c>v</c>.</returns>
f64 Round(f64 v);

/// <summary>
/// Computes the nearest integer value to <c>v</c>.
/// </summary>
/// <param name="v"> The value.</param>
/// <returns>The nearest integer value to <c>v</c>.</returns>
f32 Round(f32 v);

/// <summary>
/// Computes the nearest integer value to <c>v</c>.
/// </summary>
/// <param name="v"> The value.</param>
/// <returns>The nearest integer value to <c>v</c>.</returns>
int Round(int v);

/// <summary>
/// Rounds <c>v</c> up to the next power of 2.
/// </summary>
/// <param name="v">The value to round.</param>
/// <returns>The next power of 2.</returns>
int RoundToPow2(int v);

/// <summary>
/// </summary>
/// <param name="v">The value.</param>
/// <returns>Whether or not the value is a power of 2.</returns>
bool IsPowerOf2(int v);

/// <summary>
/// Computes the sine of <c>v</c>.
/// </summary>
/// <param name="v">The angle in radians.</param>
/// <returns>The sine.</returns>
f64 Sin(f64 v);

/// <summary>
/// Computes the sine of <c>v</c>.
/// </summary>
/// <param name="v">The angle in radians.</param>
/// <returns>The sine.</returns>
f32 Sin(f32 v);

/// <summary>
/// Computes the cosine of <c>v</c>.
/// </summary>
/// <param name="v">The angle in radians.</param>
/// <returns>The cosine.</returns>
f64 Cos(f64 v);

/// <summary>
/// Computes the cosine of <c>v</c>.
/// </summary>
/// <param name="v">The angle in radians.</param>
/// <returns>The cosine.</returns>
f32 Cos(f32 v);

/// <summary>
/// Computes the tangent of <c>v</c>.
/// </summary>
/// <param name="v">The angle in radians.</param>
/// <returns>The tangent.</returns>
f64 Tan(f64 v);

/// <summary>
/// Computes the tangent of <c>v</c>.
/// </summary>
/// <param name="v">The angle in radians.</param>
/// <returns>The tangent.</returns>
f32 Tan(f32 v);

/// <summary>
/// Computes the arc-sine for <c>v</c>.
/// </summary>
/// <param name="v">The angle in radians.</param>
/// <returns>The arc-sine.</returns>
f64 Asin(f64 v);

/// <summary>
/// Computes the arc-sine for <c>v</c>.
/// </summary>
/// <param name="v">The angle in radians.</param>
/// <returns>The arc-sine.</returns>
f32 Asin(f32 v);

/// <summary>
/// Computes the arc-cosine for <c>v</c>
/// </summary>
/// <param name="v">The angle in radians.</param>
/// <returns>The arc-cosine.</returns>
f64 Acos(f64 v);

/// <summary>
/// Computes the arc-cosine for <c>v</c>
/// </summary>
/// <param name="v">The angle in radians.</param>
/// <returns>The arc-cosine.</returns>
f32 Acos(f32 v);

/// <summary>
/// Computes the arc-tangent of <c>v</c>.
/// </summary>
/// <param name="v">The value.</param>
/// <returns>The arc-tangent.</returns>
f64 Atan(f64 v);

/// <summary>
/// Computes the arc-tangent of <c>v</c>.
/// </summary>
/// <param name="v">The value.</param>
/// <returns>The arc-tangent.</returns>
f32 Atan(f32 v);

/// <summary>
/// Computes the arc-tangent whose tan is <c>y</c>/<c>x</c>.
/// </summary>
/// <param name="y">The y value.</param>
/// <param name="x">The x value.</param>
/// <returns>The arc-tangent.</returns>
f64 Atan2(f64 y, f64 x);

/// <summary>
/// Computes the arc-tangent whose tan is <c>y</c>/<c>x</c>.
/// </summary>
/// <param name="y">The y value.</param>
/// <param name="x">The x value.</param>
/// <returns>The arc-tangent.</returns>
f32 Atan2(f32 y, f32 x);

/// <summary>
/// Computes the logarithm of the value.
/// </summary>
/// <param name="v">The value.</param>
/// <returns>The logarithm of the value.</returns>
f64 Log(f64 v);

/// <summary>
/// Computes the logarithm of the value.
/// </summary>
/// <param name="v">The value.</param>
/// <returns>The logarithm of the value.</returns>
f32 Log(f32 v);

/// <summary>
/// Computes the logarithm of the value in the specified base.
/// </summary>
/// <param name="v">The value.</param>
/// <param name="base">The base.</param>
/// <returns>The logarithm of the value in the specified base.</returns>
f64 Log(f64 v, f64 base);

/// <summary>
/// Computes the logarithm of the value in the specified base.
/// </summary>
/// <param name="v">The value.</param>
/// <param name="base">The base.</param>
/// <returns>The logarithm of the value in the specified base.</returns>
f32 Log(f32 v, f32 base);

/// <summary>
/// Computes base-10 logarithm of <c>v</c>.
/// </summary>
/// <param name="v">The value.</param>
/// <returns>The base-10 logarithm.</returns>
f64 Log10(f64 v);

/// <summary>
/// Computes base-10 logarithm of <c>v</c>.
/// </summary>
/// <param name="v">The value.</param>
/// <returns>The base-10 logarithm.</returns>
f32 Log10(f32 v);

/// <summary>
/// The modulo operator for floating-point numbers.
/// </summary>
/// <param name="n">The value.</param>
/// <param name="denom">The denominator.</param>
f64 Mod(f64 n, f64 denom);

/// <summary>
/// The modulo operator for floating-point numbers.
/// </summary>
/// <param name="n">The value.</param>
/// <param name="denom">The denominator.</param>
f32 Mod(f32 n, f32 denom);

/// <summary>
/// Returns the sign of <c>v</c>.
/// </summary>
/// <param name="v">The value.</param>
/// <returns>The sign.</returns>
int Sign(int v);

/// <summary>
/// Returns the sign of <c>v</c>.
/// </summary>
/// <param name="v">The value.</param>
/// <returns>The sign.</returns>
f64 Sign(f64 v);

/// <summary>
/// Returns the sign of <c>v</c>.
/// </summary>
/// <param name="v">The value.</param>
/// <returns>The sign.</returns>
f32 Sign(f32 v);

/// <summary>
/// Copies the sign of <c>y</c> onto <c>x</c>.
/// </summary>
/// <param name="x">The value that the sign gets copied to.</param>
/// <param name="y">The sign value.</param>
/// <returns><c>x</c> with the sign of <c>y</c>.</returns>
int CopySign(int x, int y);

/// <summary>
/// Copies the sign of <c>y</c> onto <c>x</c>.
/// </summary>
/// <param name="x">The value that the sign gets copied to.</param>
/// <param name="y">The sign value.</param>
/// <returns><c>x</c> with the sign of <c>y</c>.</returns>
f32 CopySign(f32 x, f32 y);

/// <summary>
/// Copies the sign of <c>y</c> onto <c>x</c>.
/// </summary>
/// <param name="x">The value that the sign gets copied to.</param>
/// <param name="y">The sign value.</param>
/// <returns><c>x</c> with the sign of <c>y</c>.</returns>
f64 CopySign(f64 x, f64 y);

/// <summary>
/// Calculates the smallest delta in a looping set.
/// </summary>
/// <param name="current">The current value.</param>
/// <param name="target">The target value.</param>
/// <param name="boundary">The looping boundary.</param>
/// <returns>The smallest delta.</returns>
f64 DeltaLooping(f64 current, f64 target, f64 boundary);

/// <summary>
/// Calculates the smallest delta in a looping set.
/// </summary>
/// <param name="current">The current value.</param>
/// <param name="target">The target value.</param>
/// <param name="boundary">The looping boundary.</param>
/// <returns>The smallest delta.</returns>
f32 DeltaLooping(f32 current, f32 target, f32 boundary);

/// <summary>
/// Calculates the smallest delta in a looping set.
/// </summary>
/// <param name="current">The current value.</param>
/// <param name="target">The target value.</param>
/// <param name="boundary">The looping boundary.</param>
/// <returns>The smallest delta.</returns>
int DeltaLooping(int current, int target, int boundary);

/// <summary>
/// Calculates the smallest delta between two angles.
/// </summary>
/// <param name="current">The current angle.</param>
/// <param name="target">The target angle.</param>
/// <returns>The smallest delta.</returns>
f64 DeltaAngle(f64 current, f64 target);

/// <summary>
/// Calculates the smallest delta between two angles.
/// </summary>
/// <param name="current">The current angle.</param>
/// <param name="target">The target angle.</param>
/// <returns>The smallest delta.</returns>
f32 DeltaAngle(f32 current, f32 target);

/// <summary>
/// Moves the value from <c>current</c> towards <c>target</c>.
/// Makes sure that the speed doesn't exceed <c>maxDelta</c>.
/// </summary>
/// <param name="current">The current value.</param>
/// <param name="target">The target value.</param>
/// <param name="maxDelta">The maximum delta that can be applied.</param>
/// <returns>The new value.</returns>
f64 MoveTowards(f64 current, f64 target, f64 maxDelta);

/// <summary>
/// Moves the value from <c>current</c> towards <c>target</c>.
/// Makes sure that the speed doesn't exceed <c>maxDelta</c>.
/// </summary>
/// <param name="current">The current value.</param>
/// <param name="target">The target value.</param>
/// <param name="maxDelta">The maximum delta that can be applied.</param>
/// <returns>The new value.</returns>
f32 MoveTowards(f32 current, f32 target, f32 maxDelta);

/// <summary>
/// Moves the value from <c>current</c> towards <c>target</c>.
/// Makes sure that the speed doesn't exceed <c>maxDelta</c>.
/// </summary>
/// <param name="current">The current value.</param>
/// <param name="target">The target value.</param>
/// <param name="maxDelta">The maximum delta that can be applied.</param>
/// <returns>The new value.</returns>
int MoveTowards(int current, int target, int maxDelta);

/// <summary>
/// Moves the value from <c>current</c> towards <c>target</c>.
/// Makes sure that the speed doesn't exceed <c>maxDelta</c>.
/// Makes sure wrapping around 0 and <c>boundary</c> is smooth.
/// </summary>
/// <param name="current">The current value.</param>
/// <param name="target">The target value.</param>
/// <param name="maxDelta">The maximum delta that can be applied.</param>
/// <returns>The new value.</returns>
f64 MoveTowardsLooping(f64 current, f64 target, f64 maxDelta, f64 boundary);

/// <summary>
/// Moves the value from <c>current</c> towards <c>target</c>.
/// Makes sure that the speed doesn't exceed <c>maxDelta</c>.
/// Makes sure wrapping around 0 and <c>boundary</c> is smooth.
/// </summary>
/// <param name="current">The current value.</param>
/// <param name="target">The target value.</param>
/// <param name="maxDelta">The maximum delta that can be applied.</param>
/// <returns>The new value.</returns>
f32 MoveTowardsLooping(f32 current, f32 target, f32 maxDelta, f32 boundary);

/// <summary>
/// Moves the value from <c>current</c> towards <c>target</c>.
/// Makes sure that the speed doesn't exceed <c>maxDelta</c>.
/// Makes sure wrapping around 0 and <c>boundary</c> is smooth.
/// </summary>
/// <param name="current">The current value.</param>
/// <param name="target">The target value.</param>
/// <param name="maxDelta">The maximum delta that can be applied.</param>
/// <returns>The new value.</returns>
int MoveTowardsLooping(int current, int target, int maxDelta, int boundary);

/// <summary>
/// Moves the value from <c>current</c> towards <c>target</c>.
/// Makes sure that the speed doesn't exceed <c>maxDelta</c>.
/// Makes sure wrapping around 0 and 360 is smooth.
/// </summary>
/// <param name="current">The current value.</param>
/// <param name="target">The target value.</param>
/// <param name="maxDelta">The maximum delta that can be applied.</param>
/// <returns>The new value.</returns>
f64 MoveTowardsAngle(f64 current, f64 target, f64 maxDelta);

/// <summary>
/// Moves the value from <c>current</c> towards <c>target</c>.
/// Makes sure that the speed doesn't exceed <c>maxDelta</c>.
/// Makes sure wrapping around 0 and 360 is smooth.
/// </summary>
/// <param name="current">The current value.</param>
/// <param name="target">The target value.</param>
/// <param name="maxDelta">The maximum delta that can be applied.</param>
/// <returns>The new value.</returns>
f32 MoveTowardsAngle(f32 current, f32 target, f32 maxDelta);

/// <summary>
/// Calculates the exponent.
/// </summary>
/// <param name="power">The power.</param>
/// <returns>The exponent.</returns>
f64 Exp(f64 power);

/// <summary>
/// Calculates the exponent.
/// </summary>
/// <param name="power">The power.</param>
/// <returns>The exponent.</returns>
f32 Exp(f32 power);

/// <summary>
/// Calculates the exponent.
/// </summary>
/// <param name="power">The power.</param>
/// <returns>The exponent.</returns>
int Exp(int power);

/// <summary>
/// Calculates <c>base</c> raised to the power of <c>exp</c>.
/// </summary>
/// <param name="base">The base.</param>
/// <param name="exp">The exponent.</param>
/// <returns><c>base</c> raised to the power of <c>exp</c>.</returns>
f64 Pow(f64 base, f64 exp);

/// <summary>
/// Calculates <c>base</c> raised to the power of <c>exp</c>.
/// </summary>
/// <param name="base">The base.</param>
/// <param name="exp">The exponent.</param>
/// <returns><c>base</c> raised to the power of <c>exp</c>.</returns>
f32 Pow(f32 base, f32 exp);

/// <summary>
/// Calculates <c>base</c> raised to the power of <c>exp</c>.
/// </summary>
/// <param name="base">The base.</param>
/// <param name="exp">The exponent.</param>
/// <returns><c>base</c> raised to the power of <c>exp</c>.</returns>
int Pow(int base, int exp);

/// <summary>
/// Calculates the square root of <c>v</c>.
/// </summary>
/// <param name="v">The value.</param>
/// <returns>The square root of <c>v</c>.</returns>
f64 Sqrt(f64 v);

/// <summary>
/// Calculates the square root of <c>v</c>.
/// </summary>
/// <param name="v">The value.</param>
/// <returns>The square root of <c>v</c>.</returns>
f32 Sqrt(f32 v);

/// <summary>
/// Calculates the inverse square root of <c>v</c>.
/// </summary>
/// <param name="v">The value.</param>
/// <returns>The inverse square root of <c>v</c>.</returns>
f64 InverseSqrt(f64 v);

/// <summary>
/// Calculates the inverse square root of <c>v</c>.
/// </summary>
/// <param name="v">The value.</param>
/// <returns>The inverse square root of <c>v</c>.</returns>
f32 InverseSqrt(f32 v);

/// <summary>
/// Calculates the inverse square root of <c>v</c>.
/// </summary>
/// <param name="v">The value.</param>
/// <returns>The inverse square root of <c>v</c>.</returns>
f32 FastInverseSqrt(f32 v);

}
