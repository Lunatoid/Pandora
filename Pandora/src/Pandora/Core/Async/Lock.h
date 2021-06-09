#pragma once

#include "Pandora/Core/Async/Mutex.h"

namespace pd {

class Lock {
public:
    /**
     * \brief Locks the mutex on construction.
     * Unlocks the mutex on destruction.
     * 
     * \param mutex The mutex to use.
     */
    Lock(Mutex& mutex);
    ~Lock();

private:
    Mutex& mutex;
};

}
