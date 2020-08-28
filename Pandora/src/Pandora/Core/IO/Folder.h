#pragma once

#include "Pandora/Core/Data/String.h"

namespace pd {

/// <summary>
/// <c>All</c> imposes no filter.
/// <c>OnlyFiles</c> excludes all directories.
/// <c>OnlyFolders</c> excludes all files.
/// </summary>
enum class FileFilter : byte {
    All,
    OnlyFiles,
    OnlyFolders
};

struct FolderEntry {
    bool valid = false;
    String base;
    String path;
    bool isFolder = false;
    u64 fileSize = 0;

    u64 creationTime = 0;
    u64 lastAccessTime = 0;
    u64 lastWriteTime = 0;

    void* nativeHandle = nullptr;
};

/// <summary>
/// Creates a folder and all subfolders.
/// </summary>
/// <param name="path">The path.</param>
/// <returns>How many folders were created.</returns>
int CreateFolder(StringView path);

/// <summary>
/// Checks if a folder and any subfolders exist.
/// </summary>
/// <param name="path">The path.</param>
/// <returns>Whether all folders in the path exist.</returns>
bool FolderExists(StringView path);

/// <summary>
/// Deletes a folder.
/// If <c>onlyIfEmpty</c> is <c>false</c> it will also delete any files or folders in that directory. Be careful!
/// </summary>
/// <param name="path">The path.</param>
/// <param name="onlyIfEmpty">Whether or not the folder needs to be empty first.</param>
/// <returns>Whether all folders in the path exist.</returns>
bool FolderDelete(StringView path, bool onlyIfEmpty = true);

/// <summary>
/// Gets the first file from a folder. Use <c>FolderEntry.valid</c> to check if it was successful.
/// </summary>
/// <param name="path">The path.</param>
/// <param name="entry">The entry to store the data in.</param>
void GetFirstFile(StringView path, FolderEntry& entry);

/// <summary>
/// Gets the next file from a folder. Use <c>FolderEntry.valid</c> to check if it was successful.
/// </summary>
/// <param name="entry">The entry to store the data in.</param>
void GetNextFile(FolderEntry& entry);

/// <summary>
/// Gets all files from a folder.
/// </summary>
/// <param name="path">The path.</param>
/// <param name="out">The array to store all the entries.</param>
/// <param name="recursive">Whether or not to search recursively.</param>
/// <param name="filter">The file filter to apply.</param>
void GetAllFiles(StringView path, Array<FolderEntry>& out, bool recursive = true, FileFilter filter = FileFilter::All);

}
