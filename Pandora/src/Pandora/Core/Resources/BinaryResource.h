#pragma once

#include "Pandora/Core/Resources/Resource.h"

namespace pd {

class BinaryResource final : public Resource {
public:
    static ResourceType GetType() { return ResourceType::Binary; }

    ~BinaryResource();

    /**
     * \brief Loads the binary resource from a file.
     * 
     * \param path The path to the resource.
     * \return Whether or not it loaded successfully.
     */
    virtual bool Load(StringView path) override;

    /**
     * \brief Loads the inary resource from a box.
     * 
     * \param box The box.
     * \param name The resource name.
     * \return Whether or not it loaded successfully.
     */
    virtual bool Load(Box& box, StringView name) override;

    /**
     * \brief Frees the binary data. Gets called on destruction.
     */
    void Delete();

    /**
     * \return A slice of the binary data.
     */
    Slice<byte> GetData();

private:
    Array<byte> bytes;
};

}
