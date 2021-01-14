#include "Thread.h"

#if defined(PD_WINDOWS)
#include <Windows.h>
#endif

#include "Pandora/Core/Data/Allocator.h"

#define DATA ((ThreadNativeData*)nativeData)

namespace pd {

#if defined(PD_WINDOWS)

struct ThreadNativeData {
    HANDLE handle = INVALID_HANDLE_VALUE;
    DWORD id = 0;
};

static DWORD ThreadExec(void* data) {
    Thread* thread = (Thread*)data;
    thread->InternalExecuteFunc();
    thread->InternalSetDone(true);
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
    InternalSetDone(false);
    ResumeThread(DATA->handle);
#else
#error Implement me!
#endif
}

void Thread::Terminate(int returnCode) {
#if defined(PD_WINDOWS)
    // warning C6258: using TerminateThread does not allow proper thread clean up.
    // See https://docs.microsoft.com/en-us/cpp/code-quality/c6258?view=msvc-160
    // We might need a wrapper function to check for WaitForSingleObject to see if 
    // a quit event has been received
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

#if defined(PD_WINDOWS) && defined(PD_LIB)
void Thread::InternalExecuteFunc() {
    func(data);
}

void Thread::InternalSetDone(bool val) {
    done = val;
}
#endif

}

#undef DATA
