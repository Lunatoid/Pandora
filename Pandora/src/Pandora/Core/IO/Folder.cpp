#include "Folder.h"

#if defined(PD_WINDOWS)
#include <Windows.h>
#include <shellapi.h>
#endif

#if defined(PD_LINUX)
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#endif

#include "Pandora/Core/IO/Path.h"

namespace pd {

int CreateFolder(StringView path) {
#if defined(PD_WINDOWS)
    auto createDir = [](StringView path) -> bool {
        return CreateDirectoryW(path.ToWide(), NULL);
    };
#elif defined(PD_LINUX)
    auto createDir = [](StringView path) -> bool {
        return mkdir(path.CStr(), S_IRWXU);
    };
#endif

    String dirPath(Allocator::Temporary);
    dirPath.Set(path);
    dirPath.Append('/');

    int dirCount = 0;

    int lastIndex = 0;
    Optional<int> index = -1;
    while ((index = dirPath.FindAny(u8"/\\", *index + 1)).HasValue()) {
        if (createDir(dirPath.View(0, *index))) {
            dirCount += 1;
        }
    }

    return dirCount;
}

bool FolderExists(StringView path) {
#if defined(PD_WINDOWS)
    DWORD attributes = GetFileAttributesW(path.ToWide());

    return (attributes != INVALID_FILE_ATTRIBUTES && attributes & FILE_ATTRIBUTE_DIRECTORY);
#else
    // @TODO: consider using nftw() instead, seems to be more commonly used
    DIR* dir = opendir(path.CStr());

    if (dir) {
        closedir(dir);
        return true;
    }

    return false;
#endif
}

bool FolderDelete(StringView path, bool onlyIfEmpty) {
#if defined(PD_WINDOWS)
    // Default behaviour is only delete empty directories
    if (onlyIfEmpty) {
        return RemoveDirectoryW(path.ToWide());
    }

    // Docs made it very clear that we want an absolute path
    // that exists, otherwise it might delete files in the working directory
    String absPath(Allocator::Temporary);
    GetAbsolutePath(path, absPath);

    if (!FolderExists(absPath)) {
        return false;
    }

    // For some reason this must be DOUBLE null terminated???
    // We use the sized version to include our null-terminator as part of the string
    // And then we also have the null-terminator that it adds automatically.
    wchar* widePath = UTF8ToWide(absPath.Data(), (int)absPath.SizeInBytes());

    SHFILEOPSTRUCTW fileOperation;
    fileOperation.wFunc = FO_DELETE;
    fileOperation.pFrom = widePath;
    fileOperation.fFlags = FOF_NO_UI | FOF_NOCONFIRMATION;

    return SHFileOperationW(&fileOperation) == 0;
#else
    // Default behaviour is only delete empty directories
    if (onlyIfEmpty) {
        return rmdir(path.CStr()) == 0;
    }

    // @TODO: remove all contents first
    PD_ASSERT(false, "Unimplemented functionality!");
    return false; 

#endif
}

// @TODO: option to skip dotfiles/hidden files?

#if defined(PD_WINDOWS)

#define CONVERT_TIME(x) (u64)data->x.dwLowDateTime | ((u64)data->x.dwHighDateTime << 32)

inline void SetEntryToData(FolderEntry* entry, WIN32_FIND_DATAW* data) {
    // Skip these!
    if (wcscmp(data->cFileName, L".") == 0 || wcscmp(data->cFileName, L"..") == 0) {
        GetNextFile(*entry);
        return;
    }

    entry->path.Set(WideToUTF8(data->cFileName));
    entry->isFolder = (data->dwFileAttributes | FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
    entry->fileSize = ((u64)data->nFileSizeHigh * MAXDWORD) + (u64)data->nFileSizeLow;

    entry->creationTime = CONVERT_TIME(ftCreationTime);
    entry->lastAccessTime = CONVERT_TIME(ftLastAccessTime);
    entry->lastWriteTime = CONVERT_TIME(ftLastWriteTime);
}

#undef CONVERT_TIME

#elif defined(PD_LINUX)

inline void SetEntryToData(FolderEntry* entry, struct dirent* data) {
    // Skip these!
    if (strcmp(data->d_name, ".") == 0 || strcmp(data->d_name, "..") == 0) {
        GetNextFile(*entry);
        return;
    }

    char path[PATH_MAX];
    struct stat fileStatus;

    // @TODO: find out if this works if entry->d_name is not in the working dir
    if (realpath(data->d_name, path) == NULL) {
        return;
    }

    entry->path.Set(path);

    if (stat(entry->path.CStr(), &fileStatus) == -1) return;

    entry->isFolder = data->d_type == DT_DIR; 
    entry->fileSize = fileStatus.st_size;

    // @TODO: check if the time format is consistent with Windows
    entry->creationTime = fileStatus.st_ctime;
    entry->lastAccessTime = fileStatus.st_atime;
    entry->lastWriteTime = fileStatus.st_mtime;
}

#endif

void GetFirstFile(StringView path, FolderEntry& entry) {
#if defined(PD_WINDOWS)
    // Fix path
    String realPath(Allocator::Temporary);
    realPath.Set(path);

    if (realPath.Back() != '*') {
        if (realPath.Back() != '/') {
            realPath.Append('/');
        }
        realPath.Append('*');
    }

    WIN32_FIND_DATAW realFindData;
    WIN32_FIND_DATAW* findData = &realFindData;

    entry.nativeHandle = FindFirstFileW(realPath.ToWide(), findData);
    entry.valid = entry.nativeHandle != INVALID_HANDLE_VALUE;

#elif defined(PD_LINUX)
    entry.nativeHandle = opendir(path.CStr());

    struct dirent* findData = nullptr;
    if (entry.nativeHandle != NULL) {
        findData = readdir((DIR*)entry.nativeHandle);
        entry.valid = findData != nullptr;
    }

#endif

    GetAbsolutePath(path, entry.base);
    SetEntryToData(&entry, findData);
}

void GetNextFile(FolderEntry& entry) {
#if defined(PD_WINDOWS)
    if (entry.nativeHandle == INVALID_HANDLE_VALUE) return;

    WIN32_FIND_DATAW realFindData;

    entry.valid = FindNextFileW(entry.nativeHandle, &realFindData);
    WIN32_FIND_DATAW* findData = &realFindData;
#else
    if (entry.nativeHandle == NULL) return;

    struct dirent* findData = readdir((DIR*)entry.nativeHandle);
    entry.valid = findData != NULL;
#endif

    if (entry.valid) {
        SetEntryToData(&entry, findData);
    }
}

void GetAllFiles(StringView path, Array<FolderEntry>& out, bool recursive, FileFilter filter) {
    FolderEntry entry;
    for (GetFirstFile(path, entry); entry.valid; GetNextFile(entry)) {
        if (recursive && entry.isFolder) {
            // Combine to get the next absolute path
            String newPath(Allocator::Temporary);
            newPath.Set(entry.base);
            if (newPath.Back() != '/') {
                newPath.Append('/');
            }
            newPath.Append(entry.path);

            GetAllFiles(newPath, out, recursive, filter);
        }

        // Hyper-advanced filter
        switch (filter) {
            case FileFilter::OnlyFiles:
                if (entry.isFolder) continue;
                break;

            case FileFilter::OnlyFolders:
                if (!entry.isFolder) continue;
                break;
        }

        out.Add(entry);
    }
}

}
