#pragma once

#include <time.h>

#include "Pandora/Core/Types.h"

namespace pd {

const u64 RANDOM_MAX = 0x7fffffff;
extern u64 rngState;

/// <summary>
/// Seeds the randomizer.
/// </summary>
/// <param name="seed">The seed.</param>
void SeedRandom(u64 seed);

/// <summary>
/// Seeds the randomizer with the current time.
/// </summary>
void SeedRandom();

/// <summary>
/// Generates a random integer between 0 and <c>RANDOM_MAX</c>.
/// </summary>
/// <returns>A random integer between 0 and <c>RANDOM_MAX</c>.</returns>
u64 RandomRange();

/// <summary>
/// Generates a random integer within the specified range.
/// Uses the Lehmer random number generator.
/// </summary>
/// <param name="inclMin">The inclusive minimum value.</param>
/// <param name="exclMax">The exclusive maximum value.</param>
/// <returns>A random integer within the specified range.</returns>
u64 RandomRange(u64 inclMin, u64 exclMax);

/// <summary>
/// Generates a random integer within the specified range.
/// Uses the Lehmer random number generator.
/// </summary>
/// <param name="inclMin">The inclusive minimum value.</param>
/// <param name="exclMax">The exclusive maximum value.</param>
/// <returns>A random integer within the specified range.</returns>
inline int RandomRange(int inclMin, int exclMax) {
    return (int)RandomRange(u64(inclMin), u64(exclMax));
}

/// <summary>
/// Generates a random integer within the specified range.
/// Uses the Lehmer random number generator.
/// </summary>
/// <param name="inclMin">The inclusive minimum value.</param>
/// <param name="exclMax">The exclusive maximum value.</param>
/// <returns>A random integer within the specified range.</returns>
inline i64 RandomRange(i64 inclMin, i64 exclMax) {
    return (i64)RandomRange(u64(inclMin), u64(exclMax));
}

/// <summary>
/// Generates a random double within the specified range.
/// Uses the Lehmer random number generator.
/// </summary>
/// <param name="inclMin">The inclusive minimum value.</param>
/// <param name="exclMax">The inclusive maximum value.</param>
/// <returns>A random integer within the specified range.</returns>
f64 RandomRange(f64 inclMin, f64 inclMax);

/// <summary>
/// Generates a random double between <c>0.0</c> and <c>1.0</c>.
/// </summary>
/// <returns>A random double between <c>0.0</c> and <c>1.0</c>.</returns>
f64 RandomFloat();

}
