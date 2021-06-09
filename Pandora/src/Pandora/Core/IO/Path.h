#pragma once

#include "Pandora/Core/Data/String.h"

namespace pd {

/**
 * \brief Gets the folder of the executable.
 * 
 * \param out The executable path. 
 * \return Whether or not it successfully got he path.
 */
bool GetExecutablePath(String& out);

/**
 * \brief Gets the folder of the executable.
 * 
 * \param path The new path.
 * \return Whether or not it successfully set the path.  
 */
bool SetCurrentFolder(StringView path);

/**
 * \brief Set the current working directory to the executable's directory.
 * This helper function prints error messages to the console upon failure.
 * 
 * \return Whether or not it successfully set the path.
 */
bool SetCurrentFolderToExec();

/**
 * \brief Checks whether or not the path is relative.
 * 
 * \param path The path.
 * \return Whether or not the path is relative.
 */
bool IsPathRelative(StringView path);

/**
 * \brief Converts a relative path to an absolute path based on the current working directory.
 * 
 * \param relativePath The relative path to convert.
 * \param out The output
 * \return Whether or not it converted successfully.
 */
bool GetAbsolutePath(StringView relativePath, String& out);

/**
 * \brief Checks whether or not a path points to a file.
 * 
 * \param path The path.
 * \return Whether or not it points to a file.
 */
bool IsPathFile(StringView path);

/**
 * \brief Checks whether or not a path points to a folder.
 * 
 * \param path The path.
 * \return Whether or not it points to a folder.
 */
bool IsPathFolder(StringView path);

/**
 * \brief Renames a file or folder.
 * 
 * \param path The existing path.
 * \param newPath The new path.
 * \return Whether or not it renamed successfully.
 */
bool Rename(StringView path, StringView newPath);

/**
 * \brief Returns a view of the filename + extension.
 * 
 * \param path The path.
 * \return A view to the filename + extension. Will be empty if there was none.
 */
StringView GetFileNameExt(StringView path);

/**
 * \brief Returns a view to the file extension.
 * 
 * \param path The path.
 * \param includeDot Whether or not to include the dot, e.g. `.txt` vs `txt`
 * \return A view to the file extension. Will be empty if there was none.
 */
StringView GetFileExt(StringView path, bool includeDot = true);

/**
 * \brief Returns a view to the filename without the extension.
 * 
 * \param path The path.
 * \return A view to the filename without the extension.
 */
StringView GetFileName(StringView path);

/**
 * \brief Returns a view to the folder without the filename or extension.
 * 
 * \param path The path.
 * \return A view to the folder without the filename or extension.
 */
StringView GetFolder(StringView path);

}
