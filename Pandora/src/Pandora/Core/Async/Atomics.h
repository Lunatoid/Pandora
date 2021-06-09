#pragma once

namespace pd {

class Atomic {
public:
    /**
     * \brief Initializes the atomic integer.
     * 
     * \param value The initial value.
     */
    Atomic(int value = 0) {
        val.value = value;
    }

    /**
     * \return The atomic value.
     */
    int Get();

    int operator=(int right);
    int operator++();
    int operator--();
    int operator+=(int right);
    int operator-=(int right);
    int operator*=(int right);
    int operator/=(int right);

private:
    // Functionally the same as `SDL_atomic`.
    // Written like this to avoid including the SDL headers.
    struct { int value = 0; } val;
};

}
