#pragma once

#include "Pandora/Core/Async/Mutex.h"

namespace pd {

class Lock {
public:
    Lock(Mutex& mutex);
    ~Lock();

private:
    Mutex& mutex;
};

}
