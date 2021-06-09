#pragma once

#include "Pandora/Core/Async/Mutex.h"

namespace pd {

class Condition {
public:
    Condition();
    virtual ~Condition();

    /**
     * \brief Waits until the condition is signalled.
     * 
     * \param mutex The mutex to use for the condition.
     */
    void Wait(Mutex& mutex);

    /**
     * \brief Restarts one of the threads waiting for this condition.
     */
    void Signal();

    /**
     * \brief Restarts all of the threads waiting for this condition.
     */
    void Broadcast();

    /**
     * \return The native condition handle.
     */
    void* NativeHandle();

private:
    void* nativeData;

};

}
