#pragma once

#include "Pandora/Core/Types.h"

namespace pd {

/// <summary>
/// Increments <c>dest</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <returns>The incremented variable.</returns>
i64 AtomicInc(i64& dest);

/// <summary>
/// Increments <c>dest</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <returns>The incremented variable.</returns>
i32 AtomicInc(i32& dest);

/// <summary>
/// Increments <c>dest</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <returns>The incremented variable.</returns>
i16 AtomicInc(i16& dest);

/// <summary>
/// Decrements <c>dest</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <returns>The incremented variable.</returns>
i64 AtomicDec(i64& dest);

/// <summary>
/// Decrements <c>dest</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <returns>The incremented variable.</returns>
i32 AtomicDec(i32& dest);

/// <summary>
/// Decrements <c>dest</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <returns>The incremented variable.</returns>
i16 AtomicDec(i16& dest);

/// <summary>
/// Adds <c>val</c> to the <c>dest</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="val">The iterator.</param>
/// <returns>The new variable.</returns>
i32 AtomicAdd(i32& dest, i32 val);

/// <summary>
/// Adds <c>val</c> to the <c>dest</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="val">The iterator.</param>
/// <returns>The new variable.</returns>
i64 AtomicAdd(i64& dest, i64 val);

/// <summary>
/// Subtracts <c>val</c> to the <c>dest</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="val">The iterator.</param>
/// <returns>The new variable.</returns>
i32 AtomicSub(i32& dest, i32 val);

/// <summary>
/// Subtracts <c>val</c> to the <c>dest</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="val">The iterator.</param>
/// <returns>The new variable.</returns>
i64 AtomicSub(i64& dest, i64 val);

/// <summary>
/// Swaps <c>dest</c> with <c>val</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="val">The iterator.</param>
/// <returns>The new variable.</returns>
i8 AtomicStore(i8& dest, i8 val);

/// <summary>
/// Swaps <c>dest</c> with <c>val</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="val">The iterator.</param>
/// <returns>The new variable.</returns>
bool AtomicStore(bool& dest, bool val);

/// <summary>
/// Swaps <c>dest</c> with <c>val</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="val">The iterator.</param>
/// <returns>The new variable.</returns>
i16 AtomicStore(i16& dest, i16 val);

/// <summary>
/// Swaps <c>dest</c> with <c>val</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="val">The iterator.</param>
/// <returns>The new variable.</returns>
i32 AtomicStore(i32& dest, i32 val);

/// <summary>
/// Swaps <c>dest</c> with <c>val</c> atomically.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="val">The iterator.</param>
/// <returns>The new variable.</returns>
i64 AtomicStore(i64& dest, i64 val);

/// <summary>
/// Swaps <c>dest</c> with <c>newValue</c> if <c>dest</c> is equal to <c>condition</c>.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="newValue">The new iterator.</param>
/// <param name="condition">The condition.</param>
/// <returns>The old <c>dest</c>.</returns>
i16 AtomicCompareAndSwap(i16& dest, i16 newValue, i16 condition);

/// <summary>
/// Swaps <c>dest</c> with <c>newValue</c> if <c>dest</c> is equal to <c>condition</c>.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="newValue">The new iterator.</param>
/// <param name="condition">The condition.</param>
/// <returns>The old <c>dest</c>.</returns>
i32 AtomicCompareAndSwap(i32& dest, i32 newValue, i32 condition);

/// <summary>
/// Swaps <c>dest</c> with <c>newValue</c> if <c>dest</c> is equal to <c>condition</c>.
/// </summary>
/// <param name="dest">The variable.</param>
/// <param name="newValue">The new iterator.</param>
/// <param name="condition">The condition.</param>
/// <returns>The old <c>dest</c>.</returns>
i64 AtomicCompareAndSwap(i64& dest, i64 newValue, i64 condition);

template<typename T>
class Atomic {
public:
    Atomic(T value = 0) {
        AtomicStore(t, value);
    }

    /// <summary>
    /// Swaps the iterator with <c>newValue</c> if the iterator is equal to <c>condition</c>.
    /// </summary>
    /// <param name="newValue">The new iterator.</param>
    /// <param name="condition">The condition.</param>
    /// <returns>The old iterator.</returns>
    inline T CompareAndSwap(T newValue, T condition) {
        return AtomicCompareAndSwap(t, newValue, condition);
    }
    
    /// <summary>
    /// </summary>
    /// <returns>The underlying variable.</returns>
    inline const T& Get() {
        return t;
    }

    T operator=(T right) {
        return AtomicStore(t, right);
    }

    T operator++() {
        return AtomicInc(t);
    }

    T operator--() {
        return AtomicDec(t);
    }

    T operator+=(T right) {
        return AtomicAdd(t, right);
    }

    T operator-=(T right) {
        return AtomicSub(t, right);
    }

    T operator*=(T right) {
        T result;

        for (int i = 0; i < right; i++) {
            result = AtomicAdd(t, right);
        }

        return result;
    }

    T operator/=(T right) {
        T result;

        while (t >= right) {
            result = AtomicSub(t, right);
        }

        return result;
    }

private:
    T t;
};

}
