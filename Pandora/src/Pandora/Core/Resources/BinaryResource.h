#pragma once

#include "Pandora/Core/Resources/Resource.h"

namespace pd {

class BinaryResource final : public Resource {
public:
    static ResourceType GetType() { return ResourceType::Binary; }

    ~BinaryResource();

    /// <summary>
    /// Loads a binary resource from a file.
    /// </summary>
    /// <param name="path">The path.</param>
    /// <returns>Whether or not it loaded successfully.</returns>
    virtual bool Load(StringView path) override;

    /// <summary>
    /// Loads a binary resource from a box.
    /// </summary>
    /// <param name="box">The box.</param>
    /// <param name="name">The resource name.</param>
    /// <returns>Whether or not it loaded successfully.</returns>
    virtual bool Load(Box& box, StringView name) override;
    
    /// <summary>
    /// Frees all memory related to this resource. This is called on destruction.
    /// </summary>
    void Delete();
    
    /// <summary>
    /// Returns a slice of the binary data.
    /// </summary>
    /// <returns>A slice of the binary data.</returns>
    Slice<byte> GetData();

private:
    Array<byte> bytes;
};

}
