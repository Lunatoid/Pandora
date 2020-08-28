#pragma once

#include "Pandora/Core/Types.h"

namespace pd {

/// <summary>
/// Increments <c>dest</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <returns>The incremented variable.</returns>
i64 AtomicInc(i64* dest);

/// <summary>
/// Increments <c>dest</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <returns>The incremented variable.</returns>
i32 AtomicInc(i32* dest);

/// <summary>
/// Increments <c>dest</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <returns>The incremented variable.</returns>
i16 AtomicInc(i16* dest);

/// <summary>
/// Decrements <c>dest</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <returns>The incremented variable.</returns>
i64 AtomicDec(volatile i64* dest);

/// <summary>
/// Decrements <c>dest</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <returns>The incremented variable.</returns>
i32 AtomicDec(volatile i32* dest);

/// <summary>
/// Decrements <c>dest</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <returns>The incremented variable.</returns>
i16 AtomicDec(volatile i16* dest);

/// <summary>
/// Adds <c>val</c> to the <c>dest</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="val">The value.</param>
/// <returns>The new variable.</returns>
i32 AtomicAdd(volatile i32* dest, i32 val);

/// <summary>
/// Adds <c>val</c> to the <c>dest</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="val">The value.</param>
/// <returns>The new variable.</returns>
i64 AtomicAdd(volatile i64* dest, i64 val);

/// <summary>
/// Subtracts <c>val</c> to the <c>dest</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="val">The value.</param>
/// <returns>The new variable.</returns>
i32 AtomicSub(volatile i32* dest, i32 val);

/// <summary>
/// Subtracts <c>val</c> to the <c>dest</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="val">The value.</param>
/// <returns>The new variable.</returns>
i64 AtomicSub(volatile i64* dest, i64 val);

/// <summary>
/// Swaps <c>dest</c> with <c>val</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="val">The value.</param>
/// <returns>The new variable.</returns>
i8 AtomicStore(volatile i8* dest, i8 val);

/// <summary>
/// Swaps <c>dest</c> with <c>val</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="val">The value.</param>
/// <returns>The new variable.</returns>
bool AtomicStore(volatile bool* dest, bool val);

/// <summary>
/// Swaps <c>dest</c> with <c>val</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="val">The value.</param>
/// <returns>The new variable.</returns>
i16 AtomicStore(volatile i16* dest, i16 val);

/// <summary>
/// Swaps <c>dest</c> with <c>val</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="val">The value.</param>
/// <returns>The new variable.</returns>
i32 AtomicStore(volatile i32* dest, i32 val);

/// <summary>
/// Swaps <c>dest</c> with <c>val</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="val">The value.</param>
/// <returns>The new variable.</returns>
i64 AtomicStore(volatile i64* dest, i64 val);

/// <summary>
/// Swaps <c>dest</c> with <c>newValue</c> if <c>dest</c> is equal to <c>condition</c>.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="newValue">The new value.</param>
/// <param name="condition">The condition.</param>
/// <returns>The old <c>dest</c>.</returns>
i16 AtomicCompareAndSwap(volatile i16* dest, i16 newValue, i16 condition);

/// <summary>
/// Swaps <c>dest</c> with <c>newValue</c> if <c>dest</c> is equal to <c>condition</c>.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="newValue">The new value.</param>
/// <param name="condition">The condition.</param>
/// <returns>The old <c>dest</c>.</returns>
i32 AtomicCompareAndSwap(volatile i32* dest, i32 newValue, i32 condition);

/// <summary>
/// Swaps <c>dest</c> with <c>newValue</c> if <c>dest</c> is equal to <c>condition</c>.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="newValue">The new value.</param>
/// <param name="condition">The condition.</param>
/// <returns>The old <c>dest</c>.</returns>
i64 AtomicCompareAndSwap(volatile i64* dest, i64 newValue, i64 condition);

template<typename T>
class Atomic {
public:
    Atomic(T value = 0) {
        AtomicStore(&t, value);
    }

    /// <summary>
    /// Swaps the value with <c>newValue</c> if the value is equal to <c>condition</c>.
    /// </summary>
    /// <param name="newValue">The new value.</param>
    /// <param name="condition">The condition.</param>
    /// <returns>The old value.</returns>
    inline T CompareAndSwap(T newValue, T condition) {
        return AtomicCompareAndSwap(&t, newValue, condition);
    }
    
    /// <summary>
    /// </summary>
    /// <returns>The underlying variable.</returns>
    inline volatile T* Get() {
        return &t;
    }

    T operator=(T right) {
        return AtomicStore(&t, right);
    }

    T operator++() {
        return AtomicInc(&t);
    }

    T operator--() {
        return AtomicDec(&t);
    }

    T operator+=(T right) {
        return AtomicAdd(&t, right);
    }

    T operator-=(T right) {
        return AtomicSub(&t, right);
    }

    T operator*=(T right) {
        T result;

        for (int i = 0; i < right; i++) {
            result = AtomicAdd(&t, right);
        }

        return result;
    }

    T operator/=(T right) {
        T result;

        while (t >= right) {
            result = AtomicSub(&t, right);
        }

        return result;
    }

private:
    volatile T t;
};

}
