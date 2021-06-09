#include "Thread.h"

#include "Pandora/Core/Data/Allocator.h"

#include <SDL2/SDL.h>

#define SDL_THREAD ((SDL_Thread*)nativeData)

namespace pd {

Thread::~Thread() {
    Join();
}

void Thread::Create(ThreadFunc func, void* data, StringView name) {
    nativeData = SDL_CreateThread((SDL_ThreadFunction)func, name.CStr(), data);
}

void Thread::Detach() {
    if (isDetached) return;

    SDL_DetachThread(SDL_THREAD);
    isDetached = true;
}

void Thread::Join() {
    if (isDetached) return;

    SDL_WaitThread(SDL_THREAD, nullptr);
}

void Thread::SetPriority(ThreadPriority priority) {
    SDL_ThreadPriority sdlPriority;

    switch (priority) {
        case ThreadPriority::Low: {
            sdlPriority = SDL_THREAD_PRIORITY_LOW;
            break;
        }
        case ThreadPriority::Normal: {
            sdlPriority = SDL_THREAD_PRIORITY_NORMAL;
            break;
        }
        case ThreadPriority::High: {
            sdlPriority = SDL_THREAD_PRIORITY_HIGH;
            break;
        }
    }

    SDL_SetThreadPriority(sdlPriority);
}

void* Thread::NativeHandle() {
    return nativeData;
}

}
