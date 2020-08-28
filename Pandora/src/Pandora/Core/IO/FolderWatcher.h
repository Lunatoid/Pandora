#pragma once

#include "Pandora/Core/Data/Array.h"
#include "Pandora/Core/Data/String.h"

#if defined(PD_WINDOWS)
#include <Windows.h>
#endif

namespace pd {

enum class FileChangeType : byte {
    Added,
    Modified,
    Removed
};

typedef void(WatcherOnChangeCallback)(StringView path, FileChangeType type);
typedef void(WatcherOnRenameCallback)(StringView oldPath, StringView newPath);

struct WatchEntry {
    String path;

#if defined(PD_WINDOWS)
    HANDLE handle = INVALID_HANDLE_VALUE;
#endif
};

class FolderWatcher {
public:
    FolderWatcher();
    ~FolderWatcher();
    
    /// <summary>
    /// Checks for any files changes and dispatches the events.
    /// </summary>
    /// <returns>Whether or not any changes were encountered.</returns>
    bool Check();
    
    /// <summary>
    /// Adds a folder to the watch list.
    /// </summary>
    /// <param name="path">The path to the folder.</param>
    /// <param name="watchRecursively">If <c>true</c> it will watch subfolders.</param>
    /// <returns>Whether or not it was added successfully.</returns>
    bool WatchFolder(StringView path, bool watchRecursively = true);
    
    /// <summary>
    /// Removes a folder from the watch list.
    /// </summary>
    /// <param name="path">The path to the folder.</param>
    void UnwatchFolder(StringView path);
    
    /// <summary>
    /// Registers a callback that fires on a file change.
    /// </summary>
    /// <param name="callback">The callback.</param>
    void RegisterOnChange(WatcherOnChangeCallback* callback);

    /// <summary>
    /// Unregisters a callback.
    /// </summary>
    /// <param name="callback">The callback.</param>
    void UnregisterOnChange(WatcherOnChangeCallback* callback);
    
    /// <summary>
    /// Registers a callback that fires on a file rename.
    /// </summary>
    /// <param name="callback">The callback.</param>
    void RegisterOnRename(WatcherOnRenameCallback* callback);
    
    /// <summary>
    /// Unregisters a callback.
    /// </summary>
    /// <param name="callback">The callback.</param>
    void UnregisterOnRename(WatcherOnRenameCallback* callback);
    
    /// <summary>
    /// Whether or not it should suppress all events.
    /// </summary>
    /// <param name="shouldSupress"><c>true</c> if it should suppress, <c>false</c> if not.</param>
    void SupressEvents(bool shouldSupress);
    
    /// <summary>
    /// </summary>
    /// <returns>Returns whether or not it is currently suppressing events.</returns>
    bool IsSupressed() const;
    
private:
    bool supressEvents = false;

    Array<WatchEntry> folders;
    Array<WatcherOnChangeCallback*> changeCallbacks;
    Array<WatcherOnRenameCallback*> renameCallbacks;

#if defined(PD_WINDOWS)
    void* nativeData = nullptr;
#endif
};

}
