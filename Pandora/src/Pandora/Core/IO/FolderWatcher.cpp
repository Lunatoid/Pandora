#include "FolderWatcher.h"

#include "Pandora/Core/IO/Path.h"

#define FIND_WATCH_ENTRY [&](WatchEntry& entry) {\
    return entry.path == path;\
}

#define FIND_CHANGE_CALLBACK [&](WatcherOnChangeCallback* c) {\
    return c == callback;\
}

#define FIND_RENAME_CALLBACK [&](WatcherOnRenameCallback* c) {\
    return c == callback;\
}

#define DATA ((FWNativeData*)nativeData)

namespace pd {

struct FWNativeData {
#if defined(PD_WINDOWS)
    FILE_NOTIFY_INFORMATION* notifications = nullptr;
    int notificationsCount = 0;

    OVERLAPPED overlap = {};
#endif
};

FolderWatcher::FolderWatcher() {
#if defined(PD_WINDOWS)
    nativeData = New<FWNativeData>();
    DATA->overlap = {};
    const int NOTIFY_COUNT = 128;
    DATA->notificationsCount = NOTIFY_COUNT;
    DATA->notifications = (FILE_NOTIFY_INFORMATION*)Alloc(sizeof(FILE_NOTIFY_INFORMATION) * NOTIFY_COUNT);
#endif
}

FolderWatcher::~FolderWatcher() {
#if defined(PD_WINDOWS)
    if (DATA->notifications) {
        Free(DATA->notifications);
    }
    Delete(DATA);
#endif
}

bool FolderWatcher::Check() {
#if defined(PD_WINDOWS)

    bool anyChanged = false;
    for (int i = 0; i < folders.Count(); i++) {
        DWORD written;
        bool changed = GetOverlappedResult(folders[i].handle, &DATA->overlap, &written, FALSE);

        if (!changed) continue;

        anyChanged = true;

        String path(Allocator::Temporary);
        path.Set(WideToUTF8(DATA->notifications[0].FileName, DATA->notifications[0].FileNameLength / sizeof(wchar)));
        path.Replace(u8"\\", u8"/");

        bool isRename = false;
        FileChangeType updateType;
        switch (DATA->notifications[0].Action) {
            case FILE_ACTION_ADDED:
                updateType = FileChangeType::Added;
                break;

            case FILE_ACTION_MODIFIED:
                updateType = FileChangeType::Modified;
                break;

            case FILE_ACTION_RENAMED_NEW_NAME:
            case FILE_ACTION_RENAMED_OLD_NAME:
                isRename = true;
                break;

            case FILE_ACTION_REMOVED:
                updateType = FileChangeType::Removed;
                break;

        }

        String newPath(Allocator::Temporary);
        if (isRename) {
            // Get new path
            FILE_NOTIFY_INFORMATION* nextEntry = (FILE_NOTIFY_INFORMATION*)((byte*)DATA->notifications + DATA->notifications[0].NextEntryOffset);

            newPath.Set(WideToUTF8(nextEntry->FileName, nextEntry->FileNameLength / sizeof(wchar)));
            newPath.Replace(u8"\\", u8"/");
        }

        if (!supressEvents) {
            if (isRename) {
                for (int c = 0; c < renameCallbacks.Count(); c++) {
                    renameCallbacks[i](path, newPath);
                }
            } else {
                for (int c = 0; c < changeCallbacks.Count(); c++) {
                    changeCallbacks[i](path, updateType);
                }
            }
        }

        // Consume message
        ReadDirectoryChangesW(folders[i].handle, DATA->notifications,
            DATA->notificationsCount * sizeof(FILE_NOTIFY_INFORMATION), TRUE,
            FILE_NOTIFY_CHANGE_LAST_WRITE, NULL, &DATA->overlap, NULL);
    }

    return anyChanged;
#endif
}

bool FolderWatcher::WatchFolder(StringView path, bool recursive) {
    if (!IsPathFolder(path) || folders.Find(FIND_WATCH_ENTRY) != -1) {
        return false;
    }

    WatchEntry* entry = &folders[folders.Add()];

    GetAbsolutePath(path, entry->path);

#if defined(PD_WINDOWS)
    // Windows won't allow us permission unless we use FILE_FLAG_BACKUP_SEMANTICS
    entry->handle = CreateFileW(path.ToWide(), FILE_LIST_DIRECTORY,
        FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,
        NULL, OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL);

    const int TO_WATCH = FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_SIZE;

    bool success = ReadDirectoryChangesW(entry->handle, DATA->notifications, DATA->notificationsCount * sizeof(FILE_NOTIFY_INFORMATION),
        recursive, TO_WATCH, NULL, &DATA->overlap, NULL);

    return entry->handle != INVALID_HANDLE_VALUE && success;
#endif
}

void FolderWatcher::UnwatchFolder(StringView path) {
    int index = folders.Find(FIND_WATCH_ENTRY);

    if (index == -1) return;

#if defined(PD_WINDOWS)
    CloseHandle(folders[index].handle);
#endif

    folders.Remove(index);
}

void FolderWatcher::RegisterOnChange(WatcherOnChangeCallback* callback) {
    changeCallbacks.Add(callback);
}

void FolderWatcher::UnregisterOnChange(WatcherOnChangeCallback* callback) {
    int index = changeCallbacks.Find(FIND_CHANGE_CALLBACK);

    if (index == -1) return;

    changeCallbacks.Remove(index);
}

void FolderWatcher::RegisterOnRename(WatcherOnRenameCallback* callback) {
    renameCallbacks.Add(callback);
}

void FolderWatcher::UnregisterOnRename(WatcherOnRenameCallback* callback) {
    int index = renameCallbacks.Find(FIND_RENAME_CALLBACK);

    if (index == -1) return;

    renameCallbacks.Remove(index);
}

void FolderWatcher::SupressEvents(bool shouldSupress) {
    supressEvents = shouldSupress;
}

bool FolderWatcher::IsSupressed() const {
    return supressEvents;
}

}

#undef DATA
#undef FIND_WATCH_ENTRY
#undef FIND_CHANGE_CALLBACK
#undef FIND_RENAME_CALLBACK
