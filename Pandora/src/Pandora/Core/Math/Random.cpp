#include "Random.h"

#include "Pandora/Core/Math/Math.h"

namespace pd {

Random::Random() {
    Seed();
}

Random::Random(u64 seed) {
    Seed(seed);
}

void Random::Seed(u64 seed) {
    state = Clamp<u64>(seed, 1, RANDOM_MAX);
}

void Random::Seed() {
    Seed((u64)time(NULL));
}

u64 Random::Range() {
    return Range(0ull, RANDOM_MAX);
}

u64 Random::Range(u64 inclMin, u64 exclMax) {
    u64 x = state * 48271ull % RANDOM_MAX;
    state = x;

    // Maybe in the future we can try a non-modulo version
    // if we really care about the performance of this function
    return x % (exclMax - inclMin) + inclMin;
}

f64 Random::Range(f64 inclMin, f64 inclMax) {
    f64 intPart = (f64)Range((i64)inclMin, (i64)inclMax - 1);
    return intPart + Float();
}

f64 Random::Float() {
    return (f64)Range() / (f64)RANDOM_MAX;
}

}
