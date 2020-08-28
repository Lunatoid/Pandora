#pragma once

#include "Pandora/Core/Async/Atomics.h"

namespace pd {

typedef void(ThreadFunc)(void* data);

enum class ThreadPriority : byte {
    Normal,
    Low,
    High
};

class Thread {
public:
    Thread();
    ~Thread();

    /// <summary>
    /// Initializes the thread with the specified data.
    /// </summary>
    /// <param name="func">The function to execute.</param>
    /// <param name="data">The data to pass to <c>func</c>.</param>
    /// <param name="stackSize">The stack size of the thread.</param>
    void Init(ThreadFunc func, void* data = nullptr, int stackSize = 0);

    /// <summary>
    /// Starts the thread.
    /// </summary>
    void Start();

    /// <summary>
    /// Terminates the thread.
    /// On Windows this calls <c>TerminateThread</c> which doesn't allow for proper cleanup.
    /// </summary>
    /// <param name="returnCode">The return code to pass.</param>
    void Terminate(int returnCode = 0);

    /// <summary>
    /// Joins the thread. Gets called on destruction.
    /// </summary>
    void Join();

    /// <summary>
    /// Sets the priority of the thread.
    /// </summary>
    /// <param name="priority">The priority.</param>
    void SetPriority(ThreadPriority priority);

    /// <summary>
    /// </summary>
    /// <returns>Whether or not the thread is done.</returns>
    bool IsDone();

#if defined(PD_WINDOWS)
    /// <summary>
    /// Executes the stored function.
    /// Do not call this. This is used for internal purposes.
    /// </summary>
    void ExecuteFunc();

    /// <summary>
    /// Sets the <c>done</c> bool to <c>val</c>.
    /// Do not call this. This is used for internal purposes.
    /// </summary>
    void SetDone(bool val);
#endif

protected:
    Atomic<bool> done = true;

#if defined(PD_WINDOWS)
    void* nativeData = nullptr;
    ThreadFunc* func = nullptr;
    void* data = nullptr;
#endif

};

}
