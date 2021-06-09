#include "Box.h"

#include "Pandora/Core/IO/Console.h"
#include "Pandora/Core/IO/File.h"
#include "Pandora/Core/Encoding/Compression.h"
#include "Pandora/Core/Encoding/Encryption.h"

#if defined(PD_BOX_BUILDER)
#include "Pandora/Core/Encoding/BoxBuilder.h"
#endif

namespace pd {

Box::~Box() {
    Delete();
}

bool Box::Load(StringView path) {
    Delete();

    if (!file.Open(path, FileMode::Read)) {
        CONSOLE_LOG_DEBUG("[{}Box Error{}] Failed to open file '{}' for reading\n",
                          ConColor::Red, ConColor::White, path);
        return false;
    }

    if (!file.SkipIfEqual(BOX_FILE_MAGIC)) {
        CONSOLE_LOG_DEBUG("[{}Box Error{}] Invalid format (file magic does not match)\n",
                          ConColor::Red, ConColor::White);
        return false;
    }

    byte version;
    if (file.ReadByte(&version) != 1) {
        CONSOLE_LOG_DEBUG("[{}Box Error{}] Unexpected end-of-file\n",
                          ConColor::Red, ConColor::White);
        return false;
    }

    if (version > BOX_SUPPORTED_VERSION) {
        CONSOLE_LOG_DEBUG("[{}Error{}] Unsupported version (archive version: {}, supported: {})\n",
                          ConColor::Red, ConColor::White, version, BOX_SUPPORTED_VERSION);
        return false;
    }

    // Read IV
    if (file.ReadBytes(iv, 16) != 16) {
        CONSOLE_LOG_DEBUG("[{}Error{}] Unexpected end-of-file\n",
                          ConColor::Red, ConColor::White);
        return false;
    }

    const byte BLANK_IV[16] = { 0 };
    if (!MemoryCompare(iv, (byte*)BLANK_IV, 16)) {
        // We're using encryption!
        isEncrypted = true;
        CONSOLE_LOG_DEBUG("[{}Error{}] Encrypted archives are not yet supported\n",
                          ConColor::Red, ConColor::White);
        return false;
    }

    u32 fileCount;
    if (file.Read<u32>(&fileCount) != sizeof(fileCount)) {
        CONSOLE_LOG_DEBUG("[{}Error{}] Unexpected end-of-file\n",
                          ConColor::Red, ConColor::White);
        return false;
    }

    headers.Resize((int)fileCount);
    for (u32 i = 0; i < fileCount; i++) {
        ResourceType type;
        if (file.Read(&type) != sizeof(type)) {
            CONSOLE_LOG_DEBUG("[{}Error{}] Unexpected end-of-file\n",
                              ConColor::Red, ConColor::White);
            return false;
        }

        u16 fileNameLen = 0;
        if (file.Read<u16>(&fileNameLen) != sizeof(fileNameLen)) {
            CONSOLE_LOG_DEBUG("[{}Error{}] Unexpected end-of-file\n",
                              ConColor::Red, ConColor::White);
            return false;
        }

        byte* fileName = (byte*)Alloc(fileNameLen, Allocator::Temporary);
        if (file.ReadBytes(fileName, fileNameLen) != fileNameLen) {
            CONSOLE_LOG_DEBUG("[{}Error{}] Unexpected end-of-file\n",
                              ConColor::Red, ConColor::White);
            return false;
        }

        u64 dataPosition;
        if (file.Read<u64>(&dataPosition) != sizeof(dataPosition)) {
            CONSOLE_LOG_DEBUG("[{}Error{}] Unexpected end-of-file\n",
                              ConColor::Red, ConColor::White);
            return false;
        }

        headers.Reserve(1);
        headers.Last().type = type;
        headers.Last().name.Set(fileName);
        headers.Last().position = dataPosition;
    }

    return true;
}

bool Box::LoadFromConfig(StringView configPath) {
#if defined(PD_BOX_BUILDER)
    Delete();

    builder = New<BoxBuilder>();
    return builder->AddFromConfig(configPath);
#else
    PD_ASSERT_D(false, "Can't load box from config because the box builder is not enabled");
    return false;
#endif
}

void Box::Delete() {
    headers.Delete();
    file.Close();

#if defined(PD_BOX_BUILDER)
    if (builder) {
        pd::Delete(builder);
        builder = nullptr;
    }
#endif
}

bool Box::HasResource(StringView name) {
    if (!builder) {
        return GetResourceHeader(name) != nullptr;
    } else {
#if defined(PD_BOX_BUILDER)
        Slice<BoxBuilder::StagedFile> files = builder->GetStagedFiles();

        for (int i = 0; i < files.Count(); i++) {
            if (files[i].name == name) {
                return true;
            }
        }
#endif
        return false;
    }
}

BoxHeader* Box::GetResourceHeader(StringView name) {
    if (!IsOpen() || builder) return nullptr;

    for (int i = 0; i < headers.Count(); i++) {
        if (headers[i].name == name) {
            return &headers[i];
        }
    }

    return nullptr;
}

ResourceType Box::GetResourceType(StringView name) {
    if (!IsOpen() && !builder) return ResourceType::Unknown;

    if (!builder) {
        BoxHeader* header = GetResourceHeader(name);

        return (header) ? header->type : ResourceType::Unknown;
    } else {
#if defined(PD_BOX_BUILDER)
        Slice<BoxBuilder::StagedFile> files = builder->GetStagedFiles();
        for (int i = 0; i < files.Count(); i++) {
            if (files[i].name == name) {
                return files[i].type;
            }
        }
#endif

        return ResourceType::Unknown;
    }
}

bool Box::GetResourceData(StringView name, Array<byte>& out) {
    if (!IsOpen() && !builder) return false;

    auto getData = [](Stream& in, Array<byte>& out) {
        u64 uncompressedSize;
        if (in.Read(&uncompressedSize) != sizeof(uncompressedSize)) {
            CONSOLE_LOG_DEBUG("[{}Box Error{}] Unexpected end-of-file\n",
                              ConColor::Red, ConColor::White);
            return false;
        }

        u64 compressedSize;
        if (in.Read(&compressedSize) != sizeof(compressedSize)) {
            CONSOLE_LOG_DEBUG("[{}Box Error{}] Unexpected end-of-file\n",
                              ConColor::Red, ConColor::White);
            return false;
        }

        if (compressedSize > 0) {
            Array<byte> compressed;
            compressed.Reserve((int)compressedSize);

            if (in.ReadBytes(compressed.Data(), compressedSize) != compressedSize) {
                CONSOLE_LOG_DEBUG("[{}Error{}] Unexpected end-of-file\n",
                                  ConColor::Red, ConColor::White);
                return false;
            }

            DecompressData(compressed, out);
        } else {
            out.Reserve((int)uncompressedSize);
            if (in.ReadBytes(out.Data(), uncompressedSize) != uncompressedSize) {
                CONSOLE_LOG_DEBUG("[{}Error{}] Unexpected end-of-file\n",
                                  ConColor::Red, ConColor::White);
                return false;
            }
        }
        return true;
    };

    if (!builder) {
        BoxHeader* header = GetResourceHeader(name);

        if (!header) return false;

        file.Seek(header->position, SeekOrigin::Start);
        return getData(file, out);
    } else {
#if defined(PD_BOX_BUILDER)
        MemoryStream data;

        Slice<BoxBuilder::StagedFile> files = builder->GetStagedFiles();
        BoxBuilder::StagedFile* sf = nullptr;
        for (int i = 0; i < files.Count(); i++) {
            if (files[i].name == name) {
                sf = (BoxBuilder::StagedFile*) & files[i];
                break;
            }
        }

        if (!sf) return false;

        // Don't unnecessarily compress it
        sf->compressed = false;

        builder->EncodeResource(data, *sf);
        data.Seek(0, SeekOrigin::Start);
        return getData(data, out);
#endif
    }

    return false;
}

u64 Box::GetCompressedSize(StringView name) {
    if (!IsOpen()) return 0;

    BoxHeader* header = GetResourceHeader(name);

    if (!header) return 0;

    file.Seek(header->position, SeekOrigin::Start);

    u64 decompressedSize;
    if (file.Read(&decompressedSize) != sizeof(decompressedSize)) {
        CONSOLE_LOG_DEBUG("[{}Box Error{}] Unexpected end-of-file\n",
                          ConColor::Red, ConColor::White);
        return false;
    }

    u64 compressedSize;
    if (file.Read(&compressedSize) != sizeof(compressedSize)) {
        CONSOLE_LOG_DEBUG("[{}Box Error{}] Unexpected end-of-file\n",
                          ConColor::Red, ConColor::White);
        return false;
    }

    return compressedSize;
}

u64 Box::GetUncompressedSize(StringView name) {
    if (!IsOpen()) return 0;

    BoxHeader* header = GetResourceHeader(name);

    if (!header) return 0;

    file.Seek(header->position, SeekOrigin::Start);

    u64 uncompressedSize;
    if (file.Read(&uncompressedSize) != sizeof(uncompressedSize)) {
        CONSOLE_LOG_DEBUG("[{}Box Error{}] Unexpected end-of-file\n",
                          ConColor::Red, ConColor::White);
        return false;
    }

    u64 compressedSize;
    if (file.Read(&compressedSize) != sizeof(compressedSize)) {
        CONSOLE_LOG_DEBUG("[{}Box Error{}] Unexpected end-of-file\n",
                          ConColor::Red, ConColor::White);
        return false;
    }

    return uncompressedSize;
}

bool Box::IsEncryped() const {
    return isEncrypted;
}

bool Box::IsOpen() {
    return file.IsOpen();
}

Slice<BoxHeader> Box::GetHeaders() {
    return headers;
}

}
