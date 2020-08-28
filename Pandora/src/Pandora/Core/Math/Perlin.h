#pragma once

#include "Pandora/Core/Math/Vector.h"

namespace pd {

/// <summary>
/// Generates a perlin noise value with the specified parameters.
/// </summary>
/// <param name="x">The x-coordinate.</param>
/// <param name="y">The y-coordinate.</param>
/// <param name="z">The z-coordinate.</param>
/// <returns>The perlin value between 0.0f and 1.0f.</returns>
f32 Perlin3D(f32 x, f32 y, f32 z);

/// <summary>
/// Generates a perlin noise value with the specified parameters.
/// </summary>
/// <param name="position">The xyz-coordinates.</param>
/// <returns>The perlin value between 0.0f and 1.0f.</returns>
f32 Perlin3D(Vec3 position);

/// <summary>
/// Generates a perlin noise value with the specified parameters. Uses a ridged noise function.
/// </summary>
/// <param name="x">The x-coordinate.</param>
/// <param name="y">The y-coordinate.</param>
/// <param name="z">The z-coordinate.</param>
/// <param name="octaves">The amount of octaves.</param>
/// <param name="persistance">Conrols the decrease in the amplitude of each passing octave.</param>
/// <param name="lacunarity">Controls the increase in the frequency of each passing octave.</param>
/// <param name="offset">Used to invert the ridges.</param>
/// <returns>The perlin value between 0.0f and 1.0f.</returns>
f32 RidgedPerlin3D(f32 x, f32 y, f32 z, int octaves = 6, f32 persistence = 0.5f, f32 lacunarity = 2.0f, f32 offset = 1.0f);

/// <summary>
/// Generates a perlin noise value with the specified parameters. Uses a ridged noise function.
/// </summary>
/// <param name="position">The xyz-coordinates.</param>
/// <param name="octaves">The amount of octaves.</param>
/// <param name="persistance">Conrols the decrease in the amplitude of each passing octave.</param>
/// <param name="lacunarity">Controls the increase in the frequency of each passing octave.</param>
/// <param name="offset">Used to invert the ridges.</param>
/// <returns>The perlin value between 0.0f and 1.0f.</returns>
f32 RidgedPerlin3D(Vec3 position, int octaves = 6, f32 persistence = 0.5f, f32 lacunarity = 2.0f);

/// <summary>
/// Generates a perlin noise value with the specified parameters. Uses a fractal brownian motion noise function.
/// </summary>
/// <param name="x">The x-coordinate.</param>
/// <param name="y">The y-coordinate.</param>
/// <param name="z">The z-coordinate.</param>
/// <param name="octaves">The amount of octaves.</param>
/// <param name="persistance">Conrols the decrease in the amplitude of each passing octave.</param>
/// <param name="lacunarity">Controls the increase in the frequency of each passing octave.</param>
/// <returns>The perlin value between 0.0f and 1.0f.</returns>
f32 FBMPerlin3D(f32 x, f32 y, f32 z, int octaves = 6, f32 persistence = 0.5f, f32 lacunarity = 2.0f);

/// <summary>
/// Generates a perlin noise value with the specified parameters. Uses a fractal brownian motion noise function.
/// </summary>
/// <param name="position">The xyz-coordinates.</param>
/// <param name="octaves">The amount of octaves.</param>
/// <param name="persistance">Conrols the decrease in the amplitude of each passing octave.</param>
/// <param name="lacunarity">Controls the increase in the frequency of each passing octave.</param>
/// <returns>The perlin value between 0.0f and 1.0f.</returns>
f32 FBMPerlin3D(Vec3 position, int octaves = 6, f32 persistence = 0.5f, f32 lacunarity = 2.0f);

/// <summary>
/// Generates a perlin noise value with the specified parameters. Uses a turbulence noise function.
/// </summary>
/// <param name="x">The x-coordinate.</param>
/// <param name="y">The y-coordinate.</param>
/// <param name="z">The z-coordinate.</param>
/// <param name="octaves">The amount of octaves.</param>
/// <param name="persistance">Conrols the decrease in the amplitude of each passing octave.</param>
/// <param name="lacunarity">Controls the increase in the frequency of each passing octave.</param>
/// <returns>The perlin value between 0.0f and 1.0f.</returns>
f32 TurbulencePerlin3D(f32 x, f32 y, f32 z, int octaves = 6, f32 persistence = 0.5f, f32 lacunarity = 2.0f);

/// <summary>
/// Generates a perlin noise value with the specified parameters. Uses a turbulence noise function.
/// </summary>
/// <param name="position">The xyz-coordinates.</param>
/// <param name="octaves">The amount of octaves.</param>
/// <param name="persistance">Conrols the decrease in the amplitude of each passing octave.</param>
/// <param name="lacunarity">Controls the increase in the frequency of each passing octave.</param>
/// <returns>The perlin value between 0.0f and 1.0f.</returns>
f32 TurbulencePerlin3D(Vec3 position, int octaves = 6, f32 persistence = 0.5f, f32 lacunarity = 2.0f);

}
