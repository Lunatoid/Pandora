#pragma once

#include "Pandora/Core/Types.h"
#include "Pandora/Core/Math/Vector.h"
#include "Pandora/Core/Math/Color.h"

namespace pd {

/// <summary>
/// Linearly interpolates between <c>a</c> and <c>b</c>.
/// </summary>
/// <param name="a">The Start value.</param>
/// <param name="b">The end value.</param>
/// <param name="t">The interpolation point. Clamps to <c>0.0f</c> and <c>1.0f</c>.</param>
/// <returns>The interpolated result.</returns>
f32 Lerp(f32 a, f32 b, f32 t);

/// <summary>
/// Linearly interpolates between <c>a</c> and <c>b</c>.
/// </summary>
/// <param name="a">The Start value.</param>
/// <param name="b">The end value.</param>
/// <param name="t">The interpolation point. Clamps to <c>0.0f</c> and <c>1.0f</c>.</param>
/// <returns>The interpolated result.</returns>
int Lerp(int a, int b, f32 t);

/// <summary>
/// Linearly interpolates between <c>a</c> and <c>b</c>.
/// </summary>
/// <param name="a">The Start value.</param>
/// <param name="b">The end value.</param>
/// <param name="t">The interpolation point. Clamps to <c>0.0f</c> and <c>1.0f</c>.</param>
/// <returns>The interpolated result.</returns>
Vec2 Lerp(Vec2 a, Vec2 b, f32 t);

/// <summary>
/// Linearly interpolates between <c>a</c> and <c>b</c>.
/// </summary>
/// <param name="a">The Start value.</param>
/// <param name="b">The end value.</param>
/// <param name="t">The interpolation point. Clamps to <c>0.0f</c> and <c>1.0f</c>.</param>
/// <returns>The interpolated result.</returns>
Vec3 Lerp(Vec3 a, Vec3 b, f32 t);

/// <summary>
/// Linearly interpolates between <c>a</c> and <c>b</c>.
/// </summary>
/// <param name="a">The Start value.</param>
/// <param name="b">The end value.</param>
/// <param name="t">The interpolation point. Clamps to <c>0.0f</c> and <c>1.0f</c>.</param>
/// <returns>The interpolated result.</returns>
Vec4 Lerp(Vec4 a, Vec4 b, f32 t);

/// <summary>
/// Linearly interpolates between <c>a</c> and <c>b</c>. 
/// </summary>
/// <param name="a">The Start value.</param>
/// <param name="b">The end value.</param>
/// <param name="t">The interpolation point. Does not clamp.</param>
/// <returns>The interpolated result.</returns>
f32 LerpUnclamped(f32 a, f32 b, f32 t);

/// <summary>
/// Linearly interpolates between <c>a</c> and <c>b</c>.
/// </summary>
/// <param name="a">The Start value.</param>
/// <param name="b">The end value.</param>
/// <param name="t">The interpolation point. Does not clamp.</param>
/// <returns>The interpolated result.</returns>
int LerpUnclamped(int a, int b, f32 t);

/// <summary>
/// Linearly interpolates between <c>a</c> and <c>b</c>.
/// </summary>
/// <param name="a">The Start value.</param>
/// <param name="b">The end value.</param>
/// <param name="t">The interpolation point. Does not clamp.</param>
/// <returns>The interpolated result.</returns>
Vec2 LerpUnclamped(Vec2 a, Vec2 b, f32 t);

/// <summary>
/// Linearly interpolates between <c>a</c> and <c>b</c>.
/// </summary>
/// <param name="a">The Start value.</param>
/// <param name="b">The end value.</param>
/// <param name="t">The interpolation point. Does not clamp.</param>
/// <returns>The interpolated result.</returns>
Vec3 LerpUnclamped(Vec3 a, Vec3 b, f32 t);

/// <summary>
/// Linearly interpolates between <c>a</c> and <c>b</c>.
/// </summary>
/// <param name="a">The Start value.</param>
/// <param name="b">The end value.</param>
/// <param name="t">The interpolation point. Does not clamp.</param>
/// <returns>The interpolated result.</returns>
Vec4 LerpUnclamped(Vec4 a, Vec4 b, f32 t);

/// <summary>
/// Linearly interpolates between <c>a</c> and <c>b</c>.
/// Makes sure wrapping around 0 and <c>boundary</c> is smooth.
/// </summary>
/// <param name="a">The Start value.</param>
/// <param name="b">The end value.</param>
/// <param name="t">The interpolation point. Clamps to <c>0.0f</c> and <c>1.0f</c>.</param>
/// <param name="boundary">The max value it should warp around.</param>
/// <returns>The interpolated result.</returns>
f64 LerpLooping(f64 a, f64 b, f32 t, f64 boundary);

/// <summary>
/// Linearly interpolates between <c>a</c> and <c>b</c>.
/// Makes sure wrapping around 0 and <c>boundary</c> is smooth.
/// </summary>
/// <param name="a">The Start value.</param>
/// <param name="b">The end value.</param>
/// <param name="t">The interpolation point. Clamps to <c>0.0f</c> and <c>1.0f</c>.</param>
/// <param name="boundary">The max value it should warp around.</param>
/// <returns>The interpolated result.</returns>
f32 LerpLooping(f32 a, f32 b, f32 t, f32 boundary);

/// <summary>
/// Linearly interpolates between <c>a</c> and <c>b</c>.
/// Makes sure wrapping around 0 and <c>boundary</c> is smooth.
/// </summary>
/// <param name="a">The Start value.</param>
/// <param name="b">The end value.</param>
/// <param name="t">The interpolation point. Clamps to <c>0.0f</c> and <c>1.0f</c>.</param>
/// <param name="boundary">The max value it should warp around.</param>
/// <returns>The interpolated result.</returns>
int LerpLooping(int a, int b, f32 t, int boundary);

/// <summary>
/// Linearly interpolates between <c>a</c> and <c>b</c>.
/// Makes sure wrapping around 0 and 360 is smooth.
/// </summary>
/// <param name="a">The Start value.</param>
/// <param name="b">The end value.</param>
/// <param name="t">The interpolation point. Clamps to <c>0.0f</c> and <c>1.0f</c>.</param>
/// <returns>The interpolated result.</returns>
f32 LerpAngle(f32 a, f32 b, f32 t);

/// <summary>
/// Linearly interpolates between <c>a</c> and <c>b</c>.
/// Makes sure wrapping around 0 and 1 is smooth.
/// </summary>
/// <param name="a">The Start color.</param>
/// <param name="b">The end color.</param>
/// <param name="t">The interpolation point. Clamps to <c>0.0f</c> and <c>1.0f</c>.</param>
/// <returns>The interpolated result.</returns>
Color LerpColor(Color a, Color b, f32 t);

/// <summary>
/// Calculates the interpolation point of <c>v</c> between <c>a</c> and <c>b</c>.
/// </summary>
/// <param name="a">The Start value.</param>
/// <param name="b">The end value.</param>
/// <param name="v">The value between the Start and end value.</param>
/// <returns>The interpolation point between <c>0.0f</c> and <c>1.0f</c>.</returns>
f32 InverseLerp(f32 a, f32 b, f32 v);

/// <summary>
/// Calculates the interpolation point of <c>v</c> between <c>a</c> and <c>b</c>.
/// </summary>
/// <param name="a">The Start value.</param>
/// <param name="b">The end value.</param>
/// <param name="v">The value between the Start and end value.</param>
/// <returns>The interpolation point between <c>0.0f</c> and <c>1.0f</c>.</returns>
f32 InverseLerp(int a, int b, int v);

/// <summary>
/// Calculates the interpolation point of <c>v</c> between <c>a</c> and <c>b</c>.
/// </summary>
/// <param name="a">The Start value.</param>
/// <param name="b">The end value.</param>
/// <param name="v">The value between the Start and end value.</param>
/// <returns>The interpolation point between <c>0.0f</c> and <c>1.0f</c>.</returns>
f32 InverseLerp(Vec2 a, Vec2 b, Vec2 v);

/// <summary>
/// Calculates the interpolation point of <c>v</c> between <c>a</c> and <c>b</c>.
/// </summary>
/// <param name="a">The Start value.</param>
/// <param name="b">The end value.</param>
/// <param name="v">The value between the Start and end value.</param>
/// <returns>The interpolation point between <c>0.0f</c> and <c>1.0f</c>.</returns>
f32 InverseLerp(Vec3 a, Vec3 b, Vec3 v);

/// <summary>
/// Calculates the interpolation point of <c>v</c> between <c>a</c> and <c>b</c>.
/// </summary>
/// <param name="a">The Start value.</param>
/// <param name="b">The end value.</param>
/// <param name="v">The value between the Start and end value.</param>
/// <returns>The interpolation point between <c>0.0f</c> and <c>1.0f</c>.</returns>
f32 InverseLerp(Vec4 a, Vec4 b, Vec4 v);

}
