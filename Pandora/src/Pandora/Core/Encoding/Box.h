#pragma once

#include "Pandora/Core/Data/StringView.h"
#include "Pandora/Core/IO/FileStream.h"
#include "Pandora/Core/Resources/ResourceType.h"

namespace pd {

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
    
    /// <summary>
    /// Loads the box file at the specified path.
    /// </summary>
    /// <param name="path">The path.</param>
    /// <returns>Whether or not it loaded successfully</returns>
    bool Load(StringView path);
    
    /// <summary>
    /// Frees all memory related to this box. This is called on destruction.
    /// </summary>
    void Delete();
    
    /// <summary>
    /// Returns whether or not the resource with the specified name is present in the box.
    /// </summary>
    /// <param name="name">The resource name.</param>
    /// <returns><c>true</c> if it's present, <c>false</c> if not.</returns>
    bool HasResource(StringView name);
    
    /// <summary>
    /// Returns the header for the specified resource.
    /// Will return <c>nullptr</c> if the resource does not exist.
    /// </summary>
    /// <param name="name">The resource name.</param>
    /// <returns>The header. <c>nullptr</c> if the resource does not exist.</returns>
    BoxHeader* GetResourceHeader(StringView name);
    
    /// <summary>
    /// Returns the type for the resource.
    /// Will return <c>ResourceType::Unknown</c> if the resource does not exist.
    /// </summary>
    /// <param name="name">The resource name.</param>
    /// <returns>The type. <c>ResourceType::Unknown</c> if the resource does not exist.</returns>
    ResourceType GetResourceType(StringView name);
    
    /// <summary>
    /// Decompresses the data for the specified resource.
    /// </summary>
    /// <param name="name">The resource name.</param>
    /// <param name="out">The output for the decompression.</param>
    /// <returns>Whether or not it succeeded.</returns>
    bool GetResourceData(StringView name, Array<byte>& out);

    /// <summary>
    /// Returns the compressed size of a resource in bytes.
    /// Will return 0 if the resource does not exist.
    /// </summary>
    /// <param name="name">The resource name.</param>
    /// <returns>The compressed size. 0 if the resource does not exist.</returns>
    u64 GetCompressedSize(StringView name);

    /// <summary>
    /// Returns the uncompressed size of a resource in bytes.
    /// Will return 0 if the resource does not exist.
    /// </summary>
    /// <param name="name">The resource name.</param>
    /// <returns>The uncompressed size. 0 if the resource does not exist.</returns>
    u64 GetUncompressedSize(StringView name);

    /// <summary>
    /// Returns whether or not this box is encrypted.
    /// </summary>
    /// <returns>Whether or not this box is encrypted.</returns>
    bool IsEncryped() const;
    
    /// <summary>
    /// Returns whether or not the box is open.
    /// </summary>
    /// <returns>Whether or not the box is open.</returns>
    bool IsOpen();
    
    /// <summary>
    /// Returns a slice of all headers.
    /// </summary>
    /// <returns>A slice of all headers.</returns>
    Slice<BoxHeader> GetHeaders();

private:
    FileStream file;

    Array<BoxHeader> headers;

    byte iv[16];
    bool isEncrypted = false;
};

}
