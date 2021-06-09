#pragma once

#include <time.h>

#include "Pandora/Core/Types.h"

namespace pd {

/**
 * \brief The maximum random value.
 */
const u64 RANDOM_MAX = 0x7fffffff;


/**
 * \brief A random number generator using the Lehmer RNG method.
 */
class Random {
public:
    // Unity uses Xorshift-128, might be worth looking into that.

    /**
     * \brief Seeds the random number generator with the current time.
     */
    Random();

    /**
     * \brief Seeds the random number generator.
     * 
     * \param seed The seed.
     */
    Random(u64 seed);

    /**
     * \brief Seeds the random number generator with the current time.
     */
    void Seed();

    /**
     * \brief Seeds the random number generator.
     * 
     * \param seed The seed.
     */
    void Seed(u64 seed);

    /**
     * \return A random value between 0 and `RANDOM_MAX`.
     */
    u64 Range();

    /**
     * \param inclMin The inclusive minimum value.
     * \param inclMax The exclusive maximum value.
     * \return A random integer between the specified range.
     */
    u64 Range(u64 inclMin, u64 exclMax);

    /**
     * \param inclMin The inclusive minimum value.
     * \param inclMax The inclusive maximum value.
     * \return A random floating point number between the specified range.
     */
    f64 Range(f64 inclMin, f64 inclMax);

    /**
     * \brief Converts the output of `Range()` from a `u64` to a `T`.
     * 
     * \tparam T The type to convert it to.
     * \param inclMin The inclusive minimum value.
     * \param exclMax The exclusive maximum value.
     * \return A random integral value between the specified range.
     */
    template<typename T>
    inline T Range(T inclMin, T exclMax) {
        return (T)Range((u64)inclMin, (u64)exclMax);
    }

    /**
     * \return A random floating point number between 0.0 and 1.0.
     */
    f64 Float();

private:
    u64 state = 1;
};

/**
 * \brief The global random.
 * @GLOBAL
 */
static Random random;

}
