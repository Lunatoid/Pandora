#pragma once

#include "Pandora/Core/Data/String.h"

namespace pd {

/**
 * \brief `All` imposes no filter.
 * `OnlyFiles` excludes all folders.
 * `OnlyFolders` excludes all files.
 */
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

/**
 * \brief Gets the folder of the executable.
 * 
 * \param path The path.
 * \return How many folders were created.
 */
int CreateFolder(StringView path);

/**
 * \brief Checks if a folder and any subfolders exist.
 * 
 * \param path The path.
 * \return Whether all folders in the path exist.
 */
bool FolderExists(StringView path);

/**
 * \brief Deletes a folder.
 * If `onlyIfEmpty` is false, it will also delete any files or folders in the folder. Beware!!
 * 
 * \param path The path.
 * \param onlyIfEmpty Whether to delete any contents.
 * \return Whether or not all the content was deleted.
 */
bool FolderDelete(StringView path, bool onlyIfEmpty = true);

/**
 * \brief Gets the first file in a folder.
 * Use `FolderEntry.valid` to see if it was successful.
 * 
 * \param path The path.
 * \param entry The variable to store the entry in.
 */
void GetFirstFile(StringView path, FolderEntry& entry);

/**
 * \brief Gets the next file from the folder.
 * Use `FolderEntry.valid` to check if it was successful.
 * 
 * \param entry The entry to store it in.
 * Must have been initialized with `GetFirstFile`.
 */
void GetNextFile(FolderEntry& entry);

/**
 * \brief Gets all the files from a folder.
 * 
 * \param path The path.
 * \param out The array to store the entries in.
 * \param recursive Whether or not to get all the entries recursively.
 * \param filter The filter to apply.
 */
void GetAllFiles(StringView path, Array<FolderEntry>& out, bool recursive = true, FileFilter filter = FileFilter::All);

}
