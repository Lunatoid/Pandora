#include "Lock.h"

namespace pd {

Lock::Lock(Mutex& mutex) : mutex(mutex) {
    mutex.Lock();
}

Lock::~Lock() {
    mutex.Unlock();
}

}
