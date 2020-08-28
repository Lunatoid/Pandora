#pragma once

#include "Pandora/Core/Data/String.h"

namespace pd {

/// <summary>
/// Gets the current working folder.
/// </summary>
/// <param name="out">The output for the path.</param>
/// <returns>Whether or not it got it successfully.</returns>
bool GetCurrentFolder(String& out);

/// <summary>
/// Sets the current working folder.
/// </summary>
/// <param name="path">The new path.</param>
/// <returns>Whether or not it set it successfully.</returns>
bool SetCurrentFolder(StringView path);

/// <summary>
/// Checks if a path is relative or not.
/// </summary
/// <param name="path">The path.</param>
/// <returns>Whether or not it is relative.</returns>
bool IsPathRelative(StringView path);

/// <summary>
/// Converts a relative path to an absolute path based on the current folder.
/// </summary>
/// <param name="relativePath">The relative path.</param>
/// <param name="out">The output string.</param>
/// <returns>Whether or not it was successful.</returns>
bool GetAbsolutePath(StringView relativePath, String& out);

/// <summary>
/// Gets the relative path between two absolute paths.
/// </summary>
/// <param name="fromPath">The Start path.</param>
/// <param name="toPath">The end path.</param>
/// <param name="out">The output string.</param>
/// <returns>Whether or not it was successful.</returns>
bool GetRelativePath(StringView fromPath, StringView toPath, String& out);

/// <summary>
/// Returns whether or not the path points to a file.
/// </summary>
/// <param name="path">The path.</param>
/// <returns>Whether or not the path points to a file.</returns>
bool IsPathFile(StringView path);

/// <summary>
/// Returns whether or not the path points to a folder.
/// </summary>
/// <param name="path">The path.</param>
/// <returns>Whether or not the path points to a folder.</returns>
bool IsPathFolder(StringView path);

/// <summary>
/// Renames a file or folder.
/// </summary>
/// <param name="path">The path to the file or folder to rename.</param>
/// <param name="newPath">The new path.</param>
/// <returns>Whether or not it renamed successful</returns>
bool Rename(StringView path, StringView newPath);

/// <summary>
/// Returns a view to the file name + extension. Will be an empty string if there was none.
/// </summary>
/// <param name="path">The path.</param>
/// <returns>A view to the file extension. Will be an empty string if there was none.</returns>
StringView GetFileNameExt(StringView path);

/// <summary>
/// Returns a view to the file extension. Will be an empty string if there is none.
/// </summary>
/// <param name="path">The path.</param>
/// <param name="includeDot">Whether or not to include the dot e.g. <c>.txt</c> versus <c>txt</c>.
/// <returns>A view to the file extension. Will be an empty string if there was none.</returns>
StringView GetFileExt(StringView path, bool includeDot = true);

/// <summary>
/// Returns a view to the file name without the extension.
/// </summary>
/// <param name="path">The path.</param>
/// <returns>A view to the file name without the extension.</returns>
StringView GetFileName(StringView path);

/// <summary>
/// Returns a view of the path without the file.
/// </summary>
/// <param name="path">The path.</param>
/// <returns>A view of the path without the file.</returns>
StringView GetFolder(StringView path);

}
