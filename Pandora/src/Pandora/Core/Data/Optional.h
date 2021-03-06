#pragma once

#include "Pandora/Core/Assert.h"

namespace pd {

template<typename T>
class Optional {
public:
    Optional() = default;
    template<typename ...Args>
    Optional(Args... args) : value(args...), hasValue(true) {}

    ~Optional() = default;
    
    /// <summary>
    /// Returns whether or not this optional has a iterator.
    /// </summary>
    /// <returns>Whether or not this optional has a iterator.</returns>
    bool HasValue() const {
        return hasValue;
    }

    /// <summary>
    /// Returns the iterator by reference. Will assert if there is no iterator.
    /// </summary>
    /// <returns>The iterator by reference</returns>
    T Value() {
        PD_ASSERT_D(hasValue, "Trying to access an Optional without a value");
        return value;
    }

    /// <summary>
    /// Returns the iterator if there is one. If not it will construct the iterator from the given args.
    /// </returns>
    /// <param name="args">The constructor args for the alternative iterator.</param>
    /// <returns>The iterator or a newly constructed one.</returns>
    template<typename ...Args>
    T ValueOr(Args... args) {
        if (HasValue()) {
            return value;
        } else {
            return T(args...);
        }
    }
    
    T* operator->() {
        PD_ASSERT_D(hasValue, "Trying to access an Optional without a value");
        return &value;
    }

    T operator*() {
        return Value();
    }

    operator bool() {
        return HasValue();
    }

    void operator=(const T& t) {
        hasValue = true;
        value = t;
    }

private:
    bool hasValue = false;
    T value;
};

}
