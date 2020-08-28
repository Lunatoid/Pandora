#include "Random.h"

#include "Pandora/Core/Math/Math.h"

namespace pd {

u64 rngState = 1;

void SeedRandom(u64 seed) {
    rngState = Clamp<u64>(seed, 1, RANDOM_MAX);
}

void SeedRandom() {
    SeedRandom((u64)time(NULL));
}

u64 RandomRange(u64 inclMin, u64 exclMax) {
    u64 x = rngState * 48271ull % RANDOM_MAX;
    rngState = x;

    return x % (exclMax - inclMin) + inclMin;
}

f64 RandomRange(f64 inclMin, f64 inclMax) {
    f64 intPart = f64(RandomRange((i64)inclMin, (i64)inclMax - 1));
    return intPart + RandomFloat();
}

u64 RandomRange() {
    return RandomRange(0ull, RANDOM_MAX);
}

f64 RandomFloat() {
    return f64(RandomRange()) / f64(RANDOM_MAX);
}

}
