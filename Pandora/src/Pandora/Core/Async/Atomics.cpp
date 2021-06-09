#include "Atomics.h"

#include <SDL2/SDL.h>

namespace pd {

#define SDL_ATOM ((SDL_atomic_t*)&val)

int Atomic::Get() {
    return SDL_AtomicGet(SDL_ATOM);
}

int Atomic::operator=(int right) {
    return SDL_AtomicSet(SDL_ATOM, right);
}

int Atomic::operator++() {
    return SDL_AtomicAdd(SDL_ATOM, 1);
}

int Atomic::operator--() {
    return SDL_AtomicAdd(SDL_ATOM, -1);
}

int Atomic::operator+=(int right) {
    return SDL_AtomicAdd(SDL_ATOM, right);
}

int Atomic::operator-=(int right) {
    return SDL_AtomicAdd(SDL_ATOM, -right);
}

int Atomic::operator*=(int right) {
    int result = 0;

    int value = Get();
    for (int i = 0; i < right; i++) {
        result = SDL_AtomicAdd(SDL_ATOM, value);
    }

    return result;
}

int Atomic::operator/=(int right) {
    int result = 0;

    int value = Get();
    for (int i = 0; i < right; i++) {
        result = SDL_AtomicAdd(SDL_ATOM, -value);
    }

    return result;
}

}