#include "Path.h"

#if defined(PD_WINDOWS)
#include <Windows.h>
#include <Shlwapi.h>
#include <wchar.h>
#endif

namespace pd {

bool GetCurrentFolder(String& out) {
#if defined(PD_WINDOWS)
    wchar buffer[MAX_PATH];
    bool success = GetCurrentDirectoryW(MAX_PATH, buffer) != 0;

    if (success) {
        out.Set(WideToUTF8(buffer));
        out.Replace("\\", "/");
        out.Append('/');
    }

    return success;
#else
#error Implement me!
#endif
}

bool SetCurrentFolder(StringView path) {
#if defined(PD_WINDOWS)
    return SetCurrentDirectoryW(path.ToWide());
#else
#error Implement me!
#endif
}

bool IsPathRelative(StringView path) {
#if defined(PD_WINDOWS)
    return PathIsRelativeW(path.ToWide());
#else
#error Implement me!
#endif
}

bool GetAbsolutePath(StringView relativePath, String& out) {
#if defined(PD_WINDOWS)

    if (!IsPathRelative(relativePath)) {
        // Path is already absolute
        out.Set(relativePath);
        return true;
    }

    bool success = GetCurrentFolder(out);

    if (success) {
        out.Append(relativePath);
    }

    return success;
#else
#error Implement me!
#endif
}

bool GetRelativePath(StringView fromPath, StringView toPath, String& out) {
#if defined(PD_WINDOWS)

    wchar buffer[MAX_PATH];

    wchar* fromPathWide = fromPath.ToWide();
    for (int i = 0; i < wcslen(fromPathWide); i++) {
        if (fromPathWide[i] == '/') {
            fromPathWide[i] = '\\';
        }
    }

    wchar* toPathWide = toPath.ToWide();
    for (int i = 0; i < wcslen(toPathWide); i++) {
        if (toPathWide[i] == '/') {
            toPathWide[i] = '\\';
        }
    }

    DWORD fromAttribute = GetFileAttributesW(fromPathWide);
    DWORD toAttribute = GetFileAttributesW(toPathWide);

    bool success = PathRelativePathToW(buffer, fromPathWide, fromAttribute, toPathWide, toAttribute);

    if (success) {
        out.Set(WideToUTF8(buffer));

        // Normalize slashes
        out.Replace(u8"\\", u8"/");
    }

    return success;
#else
#error Implement me!
#endif
}

bool IsPathFile(StringView path) {
#if defined(PD_WINDOWS)
    return !IsPathFolder(path);
#else
#error Implement me!
#endif
}

bool IsPathFolder(StringView path) {
#if defined(PD_WINDOWS)
    return (GetFileAttributesW(path.ToWide()) | FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
#else
#error Implement me!
#endif
}

bool Rename(StringView path, StringView newPath) {
#if defined(PD_WINDOWS)
    return _wrename(path.ToWide(), newPath.ToWide()) == 0;
#else
#error Implement me!
#endif
}

StringView GetFileNameExt(StringView path) {
    // @TODO: ideally we want to do this with just path.
    String tmpPath(Allocator::Temporary);
    tmpPath.Set(path);

    int slashIndex = tmpPath.FindReverse(u8"/");

    if (slashIndex >= 0) {
        path.Adjust(slashIndex + 1, path.Count() - slashIndex - 1);
    }

    return path;
}

StringView GetFileExt(StringView path, bool includeDot) {
    // @TODO: ideally we want to do this with just path.
    String tmpPath(Allocator::Temporary);
    tmpPath.Set(path);

    int offset = (includeDot) ? 1 : 0;

    int slashIndex = tmpPath.FindReverse(u8"/");
    int dotIndex = tmpPath.FindReverse(u8".") + offset;

    bool hasExt = dotIndex - offset >= 0;

    if (hasExt) {
        path.Adjust(dotIndex, path.Count() - dotIndex);
    }

    // If slashIndex > dotIndex that means that some other previous folder has a dot in it
    return (hasExt && dotIndex > slashIndex) ? path : "";
}

StringView GetFileName(StringView path) {

    // @TODO: ideally we want to do this with just path.
    String tmpPath(Allocator::Temporary);
    tmpPath.Set(path);

    int slashIndex = tmpPath.FindReverse(u8"/");
    int dotIndex = tmpPath.FindReverse(u8".");

    // We want 1 past the slash, this also has the effect that
    // if we didn't find a slash (-1) it will balance it to 0
    slashIndex += 1;

    if (dotIndex >= 0) {
        path.Adjust(slashIndex, dotIndex - slashIndex);
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

    int slashIndex = tmpPath.FindReverse(u8"/");

    path.Adjust(0, slashIndex);

    return path;
}

}
