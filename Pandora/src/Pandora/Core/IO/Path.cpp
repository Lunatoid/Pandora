#include "Path.h"

#include <SDL2/SDL.h>

#if defined(PD_WINDOWS)
#include <Windows.h>
#include <direct.h>
#include <Shlwapi.h>
#include <wchar.h>
#endif

#if defined(PD_LINUX)
#include <unistd.h>
#include <sys/stat.h>
#endif

#include "Pandora/Core/IO/Console.h"

#include <SDL2/SDL.h>

namespace pd {

bool GetExecutablePath(String& out) {
    char* path = SDL_GetBasePath();
    
    if (!path) return false;

    out.Set(path);
    SDL_free(path);

#if defined(PD_WINDOWS)
    out.Replace("\\", "/");
#endif

    return true;
}

bool SetCurrentFolder(StringView path) {
#if defined(PD_WINDOWS)
    return _wchdir(path.ToWide()) == 0;
#else
    return chdir(path.CStr()) == 0;
#endif
}

bool SetCurrentFolderToExec() {
    String execPath;
    if (GetExecutablePath(execPath)) {
        if (!SetCurrentFolder(execPath)) {
            return false;
        }
    } else {
        console.Log("[{}Error{}] failed to get path of executable\n", ConColor::Red, ConColor::White);
        return false;
    }
    return true;
}


bool IsPathRelative(StringView path) {
#if defined(PD_WINDOWS)
    return PathIsRelativeW(path.ToWide());
#else
    return path.Count() > 0 && path[0] == '/';
#endif
}

bool GetAbsolutePath(StringView relativePath, String& out) {
    if (!IsPathRelative(relativePath)) {
        // Path is already absolute
        out.Set(relativePath);
        return true;
    }

    // Naive way of creating an absolute path, doens't collapse things like ../../asd/foo/../
    bool success = GetExecutablePath(out);

    if (success) {
        out.Append(relativePath);
    }

    return success;
}

bool IsPathFile(StringView path) {
    return !IsPathFolder(path);
}

bool IsPathFolder(StringView path) {
#if defined(PD_WINDOWS)
    return (GetFileAttributesW(path.ToWide()) | FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
#else
    struct stat s;
    if (stat(path.CStr(), &s) == 0) {
        return s.st_mode & S_IFDIR;
    }
#endif
}

bool Rename(StringView path, StringView newPath) {
#if defined(PD_WINDOWS)
    return _wrename(path.ToWide(), newPath.ToWide()) == 0;
#else
    return rename(path.CStr(), newPath.CStr()) == 0;
#endif
}

StringView GetFileNameExt(StringView path) {
    // @TODO: ideally we want to do this with just `path`.
    String tmpPath(Allocator::Temporary);
    tmpPath.Set(path);

    Optional<int> slashIndex = tmpPath.FindReverse(u8"/");

    if (slashIndex) {
        path.Adjust(*slashIndex + 1, path.Count() - *slashIndex - 1);
    }

    return path;
}

StringView GetFileExt(StringView path, bool includeDot) {
    // @TODO: ideally we want to do this with just `path`.
    String tmpPath(Allocator::Temporary);
    tmpPath.Set(path);

    int offset = (includeDot) ? 1 : 0;

    Optional<int> slashIndex = tmpPath.FindReverse(u8"/");
    Optional<int> dotIndex = tmpPath.FindReverse(u8".").ValueOr(-1) + offset;

    bool hasExt = dotIndex.ValueOr(-1) - offset >= 0;

    if (hasExt) {
        path.Adjust(*dotIndex, path.Count() - *dotIndex);
    }

    // If slashIndex > dotIndex that means that some other previous folder has a dot in it
    return (hasExt && dotIndex.ValueOr(-1) > slashIndex.ValueOr(-1)) ? path : "";
}

StringView GetFileName(StringView path) {

    // @TODO: ideally we want to do this with just `path`.
    String tmpPath(Allocator::Temporary);
    tmpPath.Set(path);

    int slashIndex = tmpPath.FindReverse(u8"/").ValueOr(-1);
    Optional<int> dotIndex = tmpPath.FindReverse(u8".");

    // We want 1 past the slash, this also has the effect that
    // if we didn't find a slash (-1) it will balance it to 0
    slashIndex += 1;

    if (dotIndex) {
        path.Adjust(slashIndex, *dotIndex - slashIndex);
    }

    return path;
}

StringView GetFolder(StringView path) {
    if (IsPathFolder(path)) {
        return path;
    }

    // Get last slash
    String tmpPath(Allocator::Temporary);
    tmpPath.Set(path);

    int slashIndex = tmpPath.FindReverse(u8"/").ValueOr(-1);

    path.Adjust(0, slashIndex);

    return path;
}

}
