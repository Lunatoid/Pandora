#pragma once

#include "Pandora/Core/Data/String.h"

#include "Pandora/Core/Async/Mutex.h"

namespace pd {

/**
 * \return The mutex to use for IO operations.
 */
Mutex& GetIOMutex();

/**
 * \brief Reads an entire file's contents line by line into the `out` string.
 * Each line is seperated by `newlineSeperator`.
 * 
 * \param path The path.
 * \param out The output string.
 * \param newlineSeperator The seperator.
 * \return Whether or not it read the entire file successfully.
 */
bool ReadAllLines(StringView path, String& out, const uchar* newlineSeperator = u8"\n");

/**
 * \brief Reads an entire file's content into a buffer and appends it to the string.
 * Note: no encoding checks are done so make sure it's a valid UTF-8 file.
 * 
 * \param path The path.
 * \param out The output string to append to.
 * \return How many bytes were read.
 */
u64 ReadEntireFile(StringView path, String& out);

/**
 * \brief Reads an entire file's contents into an array.
 * 
 * \param path The path.
 * \param out The output array.
 * \return How many bytes were read.
 */
u64 ReadEntireFile(StringView path, Array<byte>& out);

/**
 * \brief Writes a string's contents into a file.
 * 
 * \param path The path.
 * \param contents The contents of the file.
 * \param writeBOM Whether or not to write the byte order mark.
 * \return Whether or not it successfully wrote the file.
 */
bool WriteEntireFile(StringView path, StringView contents, bool writeBOM = false);

/**
 * \brief Writes a slice's contents into a file.
 * 
 * \param path The path.
 * \param contents The contents of the file.
 * \return Whether or not it successfully wrote the file.
 */
bool WriteEntireFile(StringView path, Slice<byte> contents);

/**
 vim curly brace* \param path The path.
 * \return Whether or not the path is valid.
 */
bool FileExists(StringView path);

/**
 * \brief Deletes a file.
 * 
 * \param path The path to the file.
 * \return Whether or not the file got deleted successfully.
 */
bool FileDelete(StringView path);

/**
 * \param path The path.
 * \return The size in bytes. Will return 0 if there are any errors.
 */
u64 GetFileSize(StringView path);

}
