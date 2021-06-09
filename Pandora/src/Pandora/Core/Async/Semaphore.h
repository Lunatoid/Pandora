#pragma once

#include "Pandora/Core/Types.h"

#include "Pandora/Core/Time/Duration.h"

namespace pd {

class Semaphore {
public:
    /**
     * \brief Initializes the semaphore with an initial value.
     * 
     * \param initialValue The initial value.
     */
    Semaphore(u32 initialValue);
    ~Semaphore();

    /**
     * \brief Increases the value of the semaphore.
     */
    void Post();

    /**
     * \brief Decrements the value of the semaphore without blocking.
     * 
     * \return True if the value got decremented, false if it would block.
     */
    bool TryWait();

    /**
     * \brief Decrements the value of the semaphore.
     * If the value is zero it will wait.
     */
    void Wait();

    /**
     * \brief Decrements the value of the semaphore.
     * If the value is zero it will wait for a specified duration.
     * 
     * \param timeout The maximum timeout for the semaphore to be decremented.
     * \return True if the value got decremented, false if the timeout ran out.
     */
    bool WaitTimeout(Duration timeout);

    /**
     * \return The native semaphore handle.
     */
    void* NativeHandle();

private:
    void* nativeData = nullptr;
};

}
