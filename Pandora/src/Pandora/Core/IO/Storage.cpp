#include "Storage.h"

#include "Pandora/Core/Data/String.h"
#include "Pandora/Core/IO/File.h"
#include "Pandora/Core/IO/Folder.h"
#include "Pandora/Core/IO/Path.h"

#include <SDL2/SDL.h>

namespace pd {

// @GLOBAL
struct StorageData {
    StorageData() {
        SetPathData("AnonymousAuthor", "UnnamedApplication");
    }

    void Delete() {
        for (int i = 0; i < tempFiles.Count(); i++) {
            if (FileExists(tempFiles[i])) {
                FileDelete(tempFiles[i]);
            }
        }

        author.Delete();
        application.Delete();
        path.Delete();
        tempPath.Delete();
        cachePath.Delete();
        tempFiles.Delete();
    }

    void Init() {
        CreateFolder(path);
        CreateFolder(tempPath);
        CreateFolder(cachePath);
    }

    void SetPathData(StringView author, StringView application) {
        this->author.Set(author);
        this->application.Set(application);

        char* prefPath = SDL_GetPrefPath(author.CStr(), application.CStr());

        path.Set(prefPath);

        SDL_free(prefPath);

#if defined(PD_WINDOWS)
        path.Replace("\\", "/");
#endif

        tempPath.Set(path);
        tempPath.Append("Temp/");

        cachePath.Set(path);
        cachePath.Append("Cache/");
    }

    String author;
    String application;

    String path;
    String cachePath;
    String tempPath;

    // List of temporary files to delete
    Array<String> tempFiles;
} storageData;

void InitStorage(StringView author, StringView application) {
    storageData.SetPathData(author, application);
}

void DeleteStorage() {
    storageData.Delete();
}

bool CreateStorageFile(StringView fileName, FileStream& stream) {
    storageData.Init();

    String path;
    path.Set(GetStoragePath());
    path.Append(fileName);

    return stream.Open(path, FileMode::Write);
}

bool CreateTempStorageFile(StringView fileName, bool deleteOnClose, FileStream& stream) {
    storageData.Init();

    String path;
    path.Set(GetTempStoragePath());
    path.Append(fileName);

    bool success = stream.Open(path, FileMode::Write);
    if (success && deleteOnClose) {
        storageData.tempFiles.Add(path);
    }

    return success;
}

bool CreateCacheStorageFile(StringView fileName, u64 hash, FileStream& stream) {
    String path;
    path.Set(GetCacheStoragePath());
    path.Append(fileName);

    String hashPath;
    hashPath.Set(path);
    hashPath.Append(".hash");

    // Delete any old data
    if (FileExists(path)) {
        FileDelete(path);
    }

    if (FileExists(hashPath)) {
        FileDelete(hashPath);
    }

    FileStream fileHash(hashPath, FileMode::Write);

    if (fileHash.IsOpen()) {
        fileHash.Write(hash);
        fileHash.Close();
    }

    return stream.Open(path, FileMode::Write);
}

bool HasStorageFile(StringView fileName) {
    String path;
    path.Set(GetStoragePath());
    path.Append(fileName);

    return FileExists(path);
}

bool HasTempStorageFile(StringView fileName) {
    String path;
    path.Set(GetTempStoragePath());
    path.Append(fileName);

    return FileExists(path);
}

bool HasCacheStorageFile(StringView fileName, u64 hash) {
    String path;
    path.Set(GetCacheStoragePath());
    path.Append(fileName);

    String hashPath;
    hashPath.Set(path);
    hashPath.Append(".hash");

    if (FileExists(path) && FileExists(hashPath)) {
        FileStream hashStream(hashPath, FileMode::Read);
        if (hashStream.IsOpen()) {
            u64 fileHash;
            if (hashStream.Read(&fileHash) == sizeof(fileHash)) {
                return hash == fileHash || hash == 0;
            }
        }
    }

    return false;
}

u64 GetCacheStorageHash(StringView fileName) {
    String hashPath;
    hashPath.Set(GetCacheStoragePath());
    hashPath.Append(fileName);
    hashPath.Append(".hash");

    if (FileExists(hashPath)) {
        FileStream hashStream(hashPath, FileMode::Read);
        if (hashStream.IsOpen()) {
            u64 fileHash;
            if (hashStream.Read(&fileHash) == sizeof(fileHash)) {
                return fileHash;
            }
        }
    }

    return 0;
}

bool OpenStorageFile(StringView fileName, FileStream& stream) {
    if (HasStorageFile(fileName)) {
        String path;
        path.Set(GetStoragePath());
        path.Append(fileName);

        return stream.Open(path);
    }

    return false;
}

bool OpenTempStorageFile(StringView fileName, FileStream& stream) {
    if (HasTempStorageFile(fileName)) {
        String path;
        path.Set(GetTempStoragePath());
        path.Append(fileName);

        return stream.Open(path);
    }

    return false;
}

bool OpenCacheStorageFile(StringView fileName, u64 hash, FileStream& stream) {
    if (HasCacheStorageFile(fileName, hash)) {
        String path;
        path.Set(GetCacheStoragePath());
        path.Append(fileName);

        return stream.Open(path);
    }

    return false;
}

int CreateStorageFolders(StringView folders) {
    storageData.Init();

    String path;
    path.Set(GetStoragePath());
    path.Append(folders);

    return CreateFolder(path);
}

StringView GetStoragePath() {
    storageData.Init();
    return storageData.path;
}

StringView GetTempStoragePath() {
    storageData.Init();
    return storageData.tempPath;
}

StringView GetCacheStoragePath() {
    storageData.Init();
    return storageData.cachePath;
}

}
