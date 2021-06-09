#include "Semaphore.h"

#include <SDL2/SDL.h>

#define SDL_SEM ((SDL_semaphore*)nativeData)

namespace pd {

Semaphore::Semaphore(u32 initialValue) {
    nativeData = SDL_CreateSemaphore(initialValue);
}

Semaphore::~Semaphore() {
    SDL_DestroySemaphore(SDL_SEM);
}

void Semaphore::Post() {
    SDL_SemPost(SDL_SEM);
}

bool Semaphore::TryWait() {
    return SDL_SemTryWait(SDL_SEM) == 0;
}

void Semaphore::Wait() {
    SDL_SemWait(SDL_SEM);
}

bool Semaphore::WaitTimeout(Duration timeout) {
    return SDL_SemWaitTimeout(SDL_SEM, timeout.milliseconds) == 0;
}

void* Semaphore::NativeHandle() {
    return nativeData;
}

}
