#include "Mutex.h"

#include "Pandora/Core/Data/Allocator.h"

#include <SDL2/SDL.h>

#define SDL_MUTEX ((SDL_mutex*)nativeData)

namespace pd {

Mutex::Mutex() {
    nativeData = SDL_CreateMutex();
}

Mutex::~Mutex() {
    SDL_DestroyMutex(SDL_MUTEX);
}

void Mutex::Lock() {
    SDL_LockMutex(SDL_MUTEX);
}

bool Mutex::TryLock() {
    return SDL_TryLockMutex(SDL_MUTEX) == 0;
}

void Mutex::Unlock() {
    SDL_UnlockMutex(SDL_MUTEX);
}

void* Mutex::NativeHandle() {
    return nativeData;
}

}
