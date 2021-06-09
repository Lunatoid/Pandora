#include "Condition.h"

#include <SDL2/SDL.h>

#define SDL_COND ((SDL_cond*)nativeData)

namespace pd {

Condition::Condition() {
    nativeData = SDL_CreateCond();
}

Condition::~Condition() {
    SDL_DestroyCond(SDL_COND);
}

void Condition::Wait(Mutex& mutex) {
    SDL_CondWait(SDL_COND, (SDL_mutex*)mutex.NativeHandle());
}

void Condition::Signal() {
    SDL_CondSignal(SDL_COND);
}

void Condition::Broadcast() {
    SDL_CondBroadcast(SDL_COND);
}

void* Condition::NativeHandle() {
    return nativeData;
}

}
