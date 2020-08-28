#pragma once

#include "Pandora/Core/Data/String.h"

namespace pd {

/// <summary>
/// Reads an entire file's contents line by line into the output string.
/// Each line is seperated with the <c>newlineSeperator</c>.
/// </summary>
/// <param name="path">The path.</param>
/// <param name="out">The target string.</param>
/// <param name="newlineSeperator">The string that seperates every line.</param>
/// <returns>Whether or not it succeeded.</returns>
bool ReadAllLines(StringView path, String& out, const uchar* newlineSeperator = u8"\n");

/// <summary>
/// Reads an entire file's contents into a buffer and appends it to the string.
/// No checks are being done whether or not the file contents are valid UTF-8.
/// </summary>
/// <param name="path">The path.</param>
/// <param name="out">The target string.</param>
/// <returns>How many bytes were read.</returns>
u64 ReadEntireFile(StringView path, String& out);

/// <summary>
/// Reads an entire file's contents into an array.
/// </summary>
/// <param name="path">The path.</param>
/// <param name="out">The output.</param>
/// <returns>How many bytes were read.</returns>
u64 ReadEntireFile(StringView path, Array<byte>& out);

/// <summary>
/// Writes a string's contents into a file.
/// </summary>
/// <param name="path">The path.</param>
/// <param name="contents">The file contents.</param>
/// <param name="writeBOM">Whether or not to write the byte order mark at the Start.</param>
/// <returns>Whether or not it succeeded.</returns>
bool WriteEntireFile(StringView path, StringView contents, bool writeBOM = false);

/// <summary>
/// Writes a slice's contents into a file.
/// </summary>
/// <param name="path">The path.</param>
/// <param name="contents">The file contents.</param>
/// <param name="writeBOM">Whether or not to write the byte order mark at the Start.</param>
/// <returns>Whether or not it succeeded.</returns>
bool WriteEntireFile(StringView path, Slice<byte> contents);

/// <summary>
/// Checks if a file exists.
/// </summary>
/// <param name="path">The path.</param>
/// <returns>Whether or not it exists.</returns>
bool FileExists(StringView path);

/// <summary>
/// Deletes a file.
/// </summary>
/// <param name="path">The path.</param>
/// <returns>Whether or not the file got deleted successfully.</returns>
bool FileDelete(StringView path);

/// <summary>
/// Returns the size of a file in bytes.
/// </summary>
/// <param name="path">The path.</param>
/// <returns>The size in bytes.</returns>
u64 GetFileSize(StringView path);

}
