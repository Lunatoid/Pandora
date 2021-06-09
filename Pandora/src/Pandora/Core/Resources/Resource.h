#pragma once

#include "Pandora/Core/Resources/ResourceType.h"

#include "Pandora/Core/Encoding/Box.h"

namespace pd {

class Resource {
public:
    static ResourceType GetType() { return ResourceType::Unknown; }

    virtual ~Resource() = default;

    /**
     * \brief Loads the resource.
     * 
     * \param path The path to the file.
     * \return Whether or not it loaded successfully.
     */
    virtual bool Load(StringView path) = 0;

    /**
     * \brief Loads the resource from a box.
     * 
     * \param box The box to load it from.
     * \param name The resource name.
     * \return Whether or not it loaded correctly.
     */
    virtual bool Load(Box& box, StringView name) = 0;

    /**
     * \return The hash of the resource.
     * Calculated on loading.
     */
    virtual u64 GetHash() { return hash; }

protected:
    u64 hash = 0;
};

}
