#include "Thread.h"

#include "Pandora/Core/Data/Allocator.h"

#if defined(PD_WINDOWS)
#include <Windows.h>
#endif

#define DATA ((ThreadNativeData*)nativeData)

namespace pd {

#if defined(PD_WINDOWS)

struct ThreadNativeData {
    HANDLE handle = INVALID_HANDLE_VALUE;
    DWORD id = 0;
};

static DWORD ThreadExec(void* data) {
    Thread* thread = (Thread*)data;
    thread->ExecuteFunc();
    thread->SetDone(true);
    return 0;
}

#endif

Thread::Thread() {
    nativeData = New<ThreadNativeData>();
}

Thread::~Thread() {
#if defined(PD_WINDOWS)
    if (DATA->handle != INVALID_HANDLE_VALUE) {
        Join();
    }
    Delete(DATA);
#else
#error Implement me!
#endif
}

void Thread::Init(ThreadFunc func, void* data, int stack_size) {
#if defined(PD_WINDOWS)
    this->func = func;
    this->data = data;
    DATA->handle = CreateThread(NULL, stack_size, &ThreadExec, this, CREATE_SUSPENDED, &DATA->id);
#else
#error Implement me!
#endif
}

void Thread::Start() {
#if defined(PD_WINDOWS)
    SetDone(false);
    ResumeThread(DATA->handle);
#else
#error Implement me!
#endif
}

void Thread::Terminate(int returnCode) {
#if defined(PD_WINDOWS)
    TerminateThread(DATA->handle, returnCode);
    CloseHandle(DATA->handle);
    DATA->handle = INVALID_HANDLE_VALUE;
#else
#error Implement me!
#endif
}

void Thread::Join() {
#if defined(PD_WINDOWS)
    WaitForSingleObject(DATA->handle, INFINITE);
    CloseHandle(DATA->handle);
    DATA->handle = INVALID_HANDLE_VALUE;
#else
#error Implement me!
#endif
}

void Thread::SetPriority(ThreadPriority priority) {
#if defined(PD_WINDOWS)
    const int PRIORITIES[] = {
        THREAD_PRIORITY_NORMAL,
        THREAD_PRIORITY_ABOVE_NORMAL,
        THREAD_PRIORITY_BELOW_NORMAL
    };

    SetThreadPriority(DATA->handle, PRIORITIES[int(priority)]);
#else
#error Implement me!
#endif
}

bool Thread::IsDone() {
    return *(bool*)done.Get();
}

#if defined(PD_WINDOWS)
void Thread::ExecuteFunc() {
    func(data);
}

void Thread::SetDone(bool val) {
    done = val;
}
#endif

}

#undef DATA
