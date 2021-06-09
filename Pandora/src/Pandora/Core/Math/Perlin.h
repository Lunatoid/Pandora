#pragma once

#include "Pandora/Core/Math/Vector.h"

namespace pd {

/**
 * \param x The x-coordinate.
 * \param y The y-coordinate.
 * \param z The z-coordinate.
 * \return The perlin value between 0.0 and 1.0.
 */
f32 Perlin3D(f32 x, f32 y, f32 z);

/// <summary>
/// Generates a perlin noise value with the specified parameters.
/// </summary>
/// <param name="position">The xyz-coordinates.</param>
/// <returns>The perlin value between 0.0f and 1.0f.</returns>

/**
 * \param position The xyz-coordinate.
 * \return The perlin value between 0.0 and 1.0.
 */
f32 Perlin3D(Vec3 position);

/**
 * \brief Generates the perlin value using a ridged noise function.
 * 
 * \param x The x-coordinate.
 * \param y The y-coordinate.
 * \param z The z-coordinate.
 * \param octaves How many octaves to use.
 * \param persistence Controls the decrease in amplitude of each octave.
 * \param lacunarity Controls the increase in frequence of each octave.
 * \param offset Used to invert the ridges.
 * \return The perlin value between 0.0 and 1.0.
 */
f32 RidgedPerlin3D(f32 x, f32 y, f32 z, int octaves = 6, f32 persistence = 0.5f, f32 lacunarity = 2.0f, f32 offset = 1.0f);

/**
 * \brief Generates the perlin value using a ridged noise function.
 * 
 * \param position The xyz-coordinates.
 * \param octaves How many octaves to use.
 * \param persistence Controls the decrease in amplitude of each octave.
 * \param lacunarity Controls the increase in frequence of each octave.
 * \param offset Used to invert the ridges.
 * \return The perlin value between 0.0 and 1.0.
 */
f32 RidgedPerlin3D(Vec3 position, int octaves = 6, f32 persistence = 0.5f, f32 lacunarity = 2.0f);

/**
 * \brief Generates the perlin value using a fractal browniant motion noise function.
 * 
 * \param x The x-coordinate.
 * \param y The y-coordinate.
 * \param z The z-coordinate.
 * \param octaves How many octaves to use.
 * \param persistence Controls the decrease in amplitude of each octave.
 * \param lacunarity Controls the increase in frequence of each octave.
 * \return The perlin value between 0.0 and 1.0.
 */
f32 FBMPerlin3D(f32 x, f32 y, f32 z, int octaves = 6, f32 persistence = 0.5f, f32 lacunarity = 2.0f);

/**
 * \brief Generates the perlin value using a fractal browniant motion noise function.
 * 
 * \param position The xyz-coordinates.
 * \param octaves How many octaves to use.
 * \param persistence Controls the decrease in amplitude of each octave.
 * \param lacunarity Controls the increase in frequence of each octave.
 * \return The perlin value between 0.0 and 1.0.
 */
f32 FBMPerlin3D(Vec3 position, int octaves = 6, f32 persistence = 0.5f, f32 lacunarity = 2.0f);

/**
 * \brief Generates the perlin value using a turbulence noise function.
 * 
 * \param x The x-coordinate.
 * \param y The y-coordinate.
 * \param z The z-coordinate.
 * \param octaves How many octaves to use.
 * \param persistence Controls the decrease in amplitude of each octave.
 * \param lacunarity Controls the increase in frequence of each octave.
 * \return The perlin value between 0.0 and 1.0.
 */
f32 TurbulencePerlin3D(f32 x, f32 y, f32 z, int octaves = 6, f32 persistence = 0.5f, f32 lacunarity = 2.0f);

/**
 * \brief Generates the perlin value using a turbulence noise function.
 * 
 * \param position The xyz-coordinates.
 * \param octaves How many octaves to use.
 * \param persistence Controls the decrease in amplitude of each octave.
 * \param lacunarity Controls the increase in frequence of each octave.
 * \return The perlin value between 0.0 and 1.0.
 */
f32 TurbulencePerlin3D(Vec3 position, int octaves = 6, f32 persistence = 0.5f, f32 lacunarity = 2.0f);

}
