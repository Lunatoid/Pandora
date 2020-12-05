#pragma once

#include "Pandora/Core/Data/StringView.h"

#include "Pandora/Core/IO/FileStream.h"

namespace pd {

/// <summary>
/// Returns the path where the application can store it's data.
/// Whenever AppData is referenced in the documentation it refers to this path.
/// On Windows: <c>%LocalAppData%</c>
/// </summary>
/// <param name="out">The output.</param>
void GetAppDataFolder(String& out);

/// <summary>
/// Initializes the use of application storage.
/// Creates the following folders:
/// <para>[AppData]/[author]/[application]/</para>
/// <para>[AppData]/[author]/[application]/Temp/</para>
/// </summary>
void InitStorage(StringView author, StringView application);

/// <summary>
/// Cleans up storage data. Deletes any temporary files that were marked as so.
/// </summary>
void DeleteStorage();

/// <summary>
/// Creates a new file in the storage path.
/// </summary>
/// <param name="fileName">The name of the file to create.</param>
/// <param name="stream">The stream to use for the file.</param>
/// <returns>Whether or not the file was created successfully.</returns>
bool CreateStorageFile(StringView fileName, FileStream& stream);

/// <summary>
/// Creates a new file in the temporary storage path.
/// </summary>
/// <param name="fileName">The name of the file to create.</param>
/// <param name="deleteOnClose">Whether or not to delete the file when the application closes.</param>
/// <param name="stream">The stream to use for the file.</param>
/// <returns>Whether or not the file was created successfully.</returns>
bool CreateTempStorageFile(StringView fileName, bool deleteOnClose, FileStream& stream);

/// <summary>
/// Creates a new file in the cache storage path.
/// </summary>
/// <param name="fileName">The name of the file to create.</param>
/// <param name="hash">The hash of the data to store at the path.</param>
/// <param name="stream">The stream to use for the file.</param>
/// <returns>Whether or not the file was created successfully.</returns>
bool CreateCacheStorageFile(StringView fileName, u64 hash, FileStream& stream);

/// <summary>
/// Checks if the file exists.
/// </summary>
/// <param name="fileName">The name of the file to check.</param>
/// <returns>Whether or not the file exists.</returns>
bool HasStorageFile(StringView fileName);

/// <summary>
/// Checks if the file exists.
/// </summary>
/// <param name="fileName">The name of the file to check.</param>
/// <returns>Whether or not the file exists.</returns>
bool HasTempStorageFile(StringView fileName);

/// <summary>
/// Checks if the file exists and if the hash descriptor matches.
/// It will ignore the hash check if <c>hash</c> parameter is 0.
/// </summary>
/// <param name="fileName">The name of the file to check.</param>
/// <param name="hash">The expected hash.</param>
/// <returns>Whether or not the file exists and whether the hash matches.</returns>
bool HasCacheStorageFile(StringView fileName, u64 hash);

/// <summary>
/// Returns the hash of the file. Will return 0 if the file was not found.
/// </summary>
/// <param name="fileName">The name of the file to get the hash from.</param>
u64 GetCacheStorageHash(StringView fileName);

/// <summary>
/// Attempts to open the storage file if it exists.
/// </summary>
/// <param name="fileName">The name of the file to check.</param>
/// <returns>Whether or not the file was opened successfully.</returns>
bool OpenStorageFile(StringView filename, FileStream& stream);

/// <summary>
/// Attempts to open the temporary storage file if it exists.
/// </summary>
/// <param name="fileName">The name of the file to check.</param>
/// <returns>Whether or not the file was opened successfully.</returns>
bool OpenTempStorageFile(StringView filename, FileStream& stream);

/// <summary>
/// Attempts to open the cache file if it exists and the hash descriptor matches.
/// </summary>
/// <param name="fileName">The name of the file to check.</param>
/// <param name="hash">The expected hash.</param>
/// <returns>Whether or not the file was opened successfully.</returns>
bool OpenCacheStorageFile(StringView fileName, u64 hash, FileStream& stream);


/// <summary>
/// Creates folders in the storage path.
/// </summary>
/// <returns>How many folders were created.</returns>
int CreateStorageFolders(StringView folders);

/// <summary>
/// Returns the storage path.
/// </summary>
/// <returns>The storage path.</returns>
StringView GetStoragePath();

/// <summary>
/// Returns the temporary storage path.
/// </summary>
/// <returns>The temporary storage path.</returns>
StringView GetTempStoragePath();

/// <summary>
/// Returns the cache storage path.
/// </summary>
/// <returns>The cache storage path.</returns>
StringView GetCacheStoragePath();

}
