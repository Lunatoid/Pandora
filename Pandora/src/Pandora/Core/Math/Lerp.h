#pragma once

#include "Pandora/Core/Types.h"
#include "Pandora/Core/Math/Vector.h"
#include "Pandora/Core/Math/Color.h"

namespace pd {

/**
 * \brief Linearly interpolates between `a` and `b`.
 * 
 * \param a The start value.
 * \param b The end value.
 * \param t The interpolation point. Clamps between 1.0 and 0.0.
 * \return The interpolated result.
 */
f32 Lerp(f32 a, f32 b, f32 t);

/**
 * \brief Linearly interoplates between `a` and `b`.
 * 
 * \param a The start value.
 * \param b The end value.
 * \param t The interpolation point. Clamps between 1.0 and 0.0.
 * \return The interpolated result.
 */
int Lerp(int a, int b, f32 t);

/**
 * \brief Linearly interpolates between `a` and `b`.
 * 
 * \param a The start value.
 * \param b The end value.
 * \param t The interpolation point. Clamps between 1.0 and 0.0.
 * \return The interpolated result.
 */
Vec2 Lerp(Vec2 a, Vec2 b, f32 t);

/**
 * \brief Linearly interpolates between `a` and `b`.
 * 
 * \param a The start value.
 * \param b The end value.
 * \param t The interpolation point. Clamps between 1.0 and 0.0.
 * \return The interpolated result.
 */
Vec3 Lerp(Vec3 a, Vec3 b, f32 t);

/**
 * \brief Linearly interpolates between `a` and `b`.
 * 
 * \param a The start value.
 * \param b The end value.
 * \param t The interpolation point. Clamps between 1.0 and 0.0.
 * \return The interpolated result.
 */
Vec4 Lerp(Vec4 a, Vec4 b, f32 t);

/**
 * \brief Linearly interpolates between `a` and `b`.
 * 
 * \param a The start value.
 * \param b The end value.
 * \param t The interpolation point. Is not clamped.
 * \return The interpolated result.
 */
f32 LerpUnclamped(f32 a, f32 b, f32 t);

/**
 * \brief Linearly interpolates between `a` and `b`.
 * 
 * \param a The start value.
 * \param b The end value.
 * \param t The interpolation point. Is not clamped.
 * \return The interpolated result.
 */
int LerpUnclamped(int a, int b, f32 t);

/**
 * \brief Linearly interpolates between `a` and `b`.
 * 
 * \param a The start value.
 * \param b The end value.
 * \param t The interpolation point. Is not clamped.
 * \return The interpolated result.
 */
Vec2 LerpUnclamped(Vec2 a, Vec2 b, f32 t);

/**
 * \brief Linearly interpolates between `a` and `b`.
 * 
 * \param a The start value.
 * \param b The end value.
 * \param t The interpolation point. Is not clamped.
 * \return The interpolated result.
 */
Vec3 LerpUnclamped(Vec3 a, Vec3 b, f32 t);

/**
 * \brief Linearly interpolates between `a` and `b`.
 * 
 * \param a The start value.
 * \param b The end value.
 * \param t The interpolation point. Is not clamped.
 * \return The interpolated result.
 */
Vec4 LerpUnclamped(Vec4 a, Vec4 b, f32 t);

/**
 * \brief Linearly interpolates between `a` and `b`.
 * Makes sure wrapping around 0.0 and `boundary` is smooth.
 * 
 * \param a The start value.
 * \param b The end value.
 * \param t The interpolation point. Clamped between 0.0 and 1.0.
 * \param boundary The max wrap-around value.
 * \return The interpolated result.
 */
f64 LerpLooping(f64 a, f64 b, f32 t, f64 boundary);

/**
 * \brief Linearly interpolates between `a` and `b`.
 * Makes sure wrapping around 0.0 and `boundary` is smooth.
 * 
 * \param a The start value.
 * \param b The end value.
 * \param t The interpolation point. Clamped between 0.0 and 1.0.
 * \param boundary The max wrap-around value.
 * \return The interpolated result.
 */
f32 LerpLooping(f32 a, f32 b, f32 t, f32 boundary);

/**
 * \brief Linearly interpolates between `a` and `b`.
 * Makes sure wrapping around 0.0 and `boundary` is smooth.
 * 
 * \param a The start value.
 * \param b The end value.
 * \param t The interpolation point. Clamped between 0.0 and 1.0.
 * \param boundary The max wrap-around value.
 * \return The interpolated result.
 */
int LerpLooping(int a, int b, f32 t, int boundary);

/**
 * \brief Linearly interpolates between `a` and `b`.
 * Makes sure wrapping around 0.0 and 360.0 is smooth.
 * 
 * \param a The start value.
 * \param b The end value.
 * \param t The interpolation point. Clamped between 0.0 and 1.0.
 * \return The interpolated result.
 */
f32 LerpAngle(f32 a, f32 b, f32 t);

/**
 * \brief Linearly interpolates between `a` and `b`.
 * Makes sure wrapping around 0.0 and 1.0 is smooth.
 * 
 * \param a The start value.
 * \param b The end value.
 * \param t The interpolation point. Clamped between 0.0 and 1.0.
 * \return The interpolated result.
 */
Color LerpColor(Color a, Color b, f32 t);

/**
 * \brief Calculates the interpolation point of `v` between `a` and `b`.
 * 
 * \param a The start value.
 * \param b The end value.
 * \param v The value between `a` and `b`.
 * \return The interpolation point.
 */
f32 InverseLerp(f32 a, f32 b, f32 v);

/**
 * \brief Calculates the interpolation point of `v` between `a` and `b`.
 * 
 * \param a The start value.
 * \param b The end value.
 * \param v The value between `a` and `b`.
 * \return The interpolation point.
 */
f32 InverseLerp(int a, int b, int v);

/**
 * \brief Calculates the interpolation point of `v` between `a` and `b`.
 * 
 * \param a The start value.
 * \param b The end value.
 * \param v The value between `a` and `b`.
 * \return The interpolation point.
 */
f32 InverseLerp(Vec2 a, Vec2 b, Vec2 v);

/**
 * \brief Calculates the interpolation point of `v` between `a` and `b`.
 * 
 * \param a The start value.
 * \param b The end value.
 * \param v The value between `a` and `b`.
 * \return The interpolation point.
 */
f32 InverseLerp(Vec3 a, Vec3 b, Vec3 v);

/**
 * \brief Calculates the interpolation point of `v` between `a` and `b`.
 * 
 * \param a The start value.
 * \param b The end value.
 * \param v The value between `a` and `b`.
 * \return The interpolation point.
 */
f32 InverseLerp(Vec4 a, Vec4 b, Vec4 v);

}
