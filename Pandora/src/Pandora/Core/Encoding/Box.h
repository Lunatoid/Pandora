#pragma once

#include "Pandora/Core/Data/StringView.h"
#include "Pandora/Core/IO/FileStream.h"
#include "Pandora/Core/Resources/ResourceType.h"

namespace pd {

#if defined(PD_BOX_BUILDER)
class BoxBuilder;
#endif

// File constants
const byte BOX_FILE_MAGIC_RAW[] = { 'B', 'O', 'X', '\n' };
const Slice<byte> BOX_FILE_MAGIC = Slice<byte>((byte*)BOX_FILE_MAGIC_RAW, sizeof(BOX_FILE_MAGIC_RAW));
const byte BOX_VERSION = 1;
const byte BOX_SUPPORTED_VERSION = 1;

struct BoxHeader {
    ~BoxHeader() = default;

    String name;
    ResourceType type;
    u64 position;
};

class Box {
public:
    ~Box();

    /**
     * \brief Loads the box file at the specified path.
     * 
     * \param path The path.
     * \return Whether or not it loaded successfully.
     */
    bool Load(StringView path);

    /**
     * \brief Loads the resources from a config file instead of a .box file.
     * This config file is the same one that the BoxBuilder uses.
     * 
     * BoxBuilder must be enabled to use this function.
     * 
     * \param configPath The path to the config file.
     * \return Whether or not it loaded successfully.
     */
    bool LoadFromConfig(StringView configPath);

    /**
     * \brief Frees the memory. Gets called on destruction.
     * 
     */
    void Delete();

    /**
     * \param name The resource name.
     * \return Whether or not the specified resource is present.
     */
    bool HasResource(StringView name);

    /**
     * \param name The resource name.
     * \return A pointer to the box header of the specified resource.
     * Returns a nullptr when in config mode or the resource does not exist.
     */
    BoxHeader* GetResourceHeader(StringView name);

    /**
     * \param name The resource name.
     * \return The type of the resource. Will return ResourceType::Unknown if
     * the resource does not exist.
     */
    ResourceType GetResourceType(StringView name);

    /**
     * \brief Reads and decompresses the raw data for a resource.
     * 
     * \param name The resource name.
     * \param out The output array.
     * \return Whether or not it read and decompressed successfully.
     */
    bool GetResourceData(StringView name, Array<byte>& out);

    /**
     * \param name The resource name.
     * \return The compressed size in bytes of the resource.
     * Will return 0 if the resource does not exist.
     */
    u64 GetCompressedSize(StringView name);

    /**
     * \param name The resource name.
     * \return The uncompressed size in bytes of the resource.
     * Will reutrn 0 if the resource does not exist.
     */
    u64 GetUncompressedSize(StringView name);

    /**
     * \return Whether or not the box is encrypted.
     * This feature is currently not supported.
     */
    bool IsEncryped() const;

    /**
     * \return Whether or not the box is currently open.
     */
    bool IsOpen();

    /**
     * \return All the resource headers.
     * Does not work in config mode.
     */
    Slice<BoxHeader> GetHeaders();

private:
    // BOX mode
    FileStream file;

    Array<BoxHeader> headers;

    byte iv[16];
    bool isEncrypted = false;

    // Config mode
#if defined(PD_BOX_BUILDER)
    BoxBuilder* builder = nullptr;
#else
    void* builder = nullptr;
#endif
};

}
