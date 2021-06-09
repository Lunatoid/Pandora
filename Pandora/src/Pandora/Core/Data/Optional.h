#pragma once

#include "Pandora/Core/Assert.h"

namespace pd {

template<typename T>
class Optional {
public:
    Optional() = default;

    /**
     * \brief Constructs a new optional object.
     * 
     * \param value The value.
     */
    Optional(const T& value) {
        SetValue(value);
    }

    ~Optional() {
        Clear();
    }

    /**
     * \return Whether or not it has a value.
     */
    bool HasValue() const {
        return hasValue;
    }

    /**
     * \brief If the optional has a value, it destructs it.
     * If there is no value, it does nothing. Gets called on destruction.
     */
    void Clear() {
        if (HasValue()) {
            Value().~T();
            hasValue = false;
        }
    }

    /**
     * \return Gets the value.
     */
    T& Value() {
        PD_ASSERT_D(hasValue, "Trying to access an Optional<T> without a value");
        return *((T*)&rawValue);
    }

    /**
     * \brief 
     * 
     * \param args The constructor arguments.
     * \return The value if there is one. If there is none, it will construct one.
     */
    template<typename ...Args>
    T ValueOr(Args&&... args) {
        if (HasValue()) {
            return Value();
        } else {
            return T(args...);
        }
    }

    /**
     * \brief Sets the value.
     * 
     * \param args The constructor arguments.
     */
    template<typename ...Args>
    void SetValue(Args&&... args) {
        if (HasValue()) {
            Value() = T(std::forward<Args>(args)...);
        } else {
            hasValue = true;
            new (&rawValue) T(std::forward<Args>(args)...);
        }
    }

    void SetValue(const T& t) {
        if (HasValue()) {
            Value() = t;
        } else {
            hasValue = true;
            new (&rawValue) T(t);
        }
    }

    T* operator->() {
        PD_ASSERT_D(hasValue, "Trying to access an Optional<T> without a value");
        return &Value();
    }

    T& operator*() {
        return Value();
    }

    explicit operator bool() {
        return HasValue();
    }

    void operator=(const T& t) {
        SetValue(t);
    }

private:
    bool hasValue = false;

    // We want to avoid calling the constructor/destructor of T
    u8 rawValue[sizeof(T)] = {};
};

}
