#pragma once

#include "Pandora/Core/Async/Atomics.h"
#include "Pandora/Core/Data/StringView.h"

namespace pd {

typedef void(ThreadFunc)(void* data);

enum class ThreadPriority : byte {
    Normal,
    Low,
    High
};

class Thread {
public:
    virtual ~Thread();

    /**
     * \brief Creates and starts the thread.
     * 
     * \param func The thread function.
     * \param data Data to pass to the thread function.
     * \param name The name of the thread.
     */
    void Create(ThreadFunc func, void* data = nullptr, StringView name = "Pandora Thread");

    /**
     * \brief Detaches the thread so that it will finish on it's own.
     * You can not call `Join()` on a detached thread.
     */
    void Detach();

    /**
     * \brief Joins the thread.
     * Gets called on destruction unless detached.
     */
    void Join();

    /**
     * \brief Sets the priority of the thread.
     * 
     * \param priority The new priorty.
     */
    void SetPriority(ThreadPriority priority);

    /**
     * \return The native thread handle.
     */
    void* NativeHandle();

protected:
    void* nativeData = nullptr;
    bool isDetached = false;

};

}
