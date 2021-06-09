#pragma once

#include "Pandora/Core/Data/StringView.h"

#include "Pandora/Core/IO/FileStream.h"

namespace pd {

/**
 * \brief Initializes the use of application storage.
 * 
 * \param author The author of this application.
 * \param application The application name.
 */
void InitStorage(StringView author, StringView application);

/**
 * \brief Cleans up the storage data. Also deletes any temporary files that are marked for deletion.
 * 
 */
void DeleteStorage();

/**
 * \brief Creates a new file in the application storage.
 * 
 * \param fileName The filename of the file.
 * \param stream The stream to use for the file.
 * \return Whether or not the file was created successfully.
 */
bool CreateStorageFile(StringView fileName, FileStream& stream);

/**
 * \brief Creates a new file in the temporary application storage.
 * 
 * \param fileName The filename of the file.
 * \param deleteOnClose Whether or not to delete the file when the application closes.
 * \param stream The stream to use for this file.
 * \return Whether or not the file was created successfully.
 */
bool CreateTempStorageFile(StringView fileName, bool deleteOnClose, FileStream& stream);

/**
 * \brief Creates a new file in the cache application storage.
 * 
 * \param fileName The filename of the file.
 * \param hash The hash of the file. Used for getting/invalidating cache.
 * \param stream The stream to use for this file.
 * \return Whether or not the file was created successfully.
 */
bool CreateCacheStorageFile(StringView fileName, u64 hash, FileStream& stream);

/**
 * \brief Checks if a file exists in the storage path.
 * 
 * \param fileName The filename of the file.
 * \return Whether or not the file exists.
 */
bool HasStorageFile(StringView fileName);

/**
 * \brief Checks if a file exists in the temporary storage path.
 * 
 * \param fileName The filename of the file.
 * \return Whether or not the file exists.
 */
bool HasTempStorageFile(StringView fileName);

/**
 * \brief Checks if the file exists in the cache storage path.
 * It will also check the hash. If the hash check fails it will also return false.
 * To disable the hash check, pass 0 as the hash.
 * 
 * \param fileName The filename of the file.
 * \param hash The expected hash.
 * \return Whether or not the file exists and matches the hash.
 */
bool HasCacheStorageFile(StringView fileName, u64 hash);

/**
 * \param fileName The filename of the file.
 * \return The hash of the file. Will return 0 if the hash file doesn't exist.
 */
u64 GetCacheStorageHash(StringView fileName);

/**
 * \brief Attempts to open a file in the storage path, if it exists.
 * 
 * \param filename The filename of the file.
 * \param stream The stream to use to open the file.
 * \return Whether or not the file was opened successfully.
 */
bool OpenStorageFile(StringView filename, FileStream& stream);

/**
 * \brief Attempts to open a file in the temporary storage path, if it exists.
 * 
 * \param filename The filename of the file.
 * \param stream The stream to use to open the file.
 * \return Whether or not the file was opened successfully.
 */
bool OpenTempStorageFile(StringView filename, FileStream& stream);

/**
 * \brief Attempts to open a file in the cache storage path, if it exists and matches the hash.
 * To disable the hash check, pass 0 as the hash.
 * 
 * \param fileName The filename of the file.
 * \param hash The expected hash. 
 * \param stream The stream to use to open the file.
 * \return Whether or not the file was opened successfully.
 */
bool OpenCacheStorageFile(StringView fileName, u64 hash, FileStream& stream);

/**
 * \brief Creates a folder and any subfolders in the storage path.
 * 
 * \param folders The path to create.
 * \return How many folders were created.
 */
int CreateStorageFolders(StringView folders);

/**
 * \return The storage path.
 */
StringView GetStoragePath();

/**
 * \return The temporary storage path.
 */
StringView GetTempStoragePath();

/**
 * \return The cache storage path.
 */
StringView GetCacheStoragePath();

}
