#pragma once

#include "Pandora/Core/Resources/ResourceType.h"

#include "Pandora/Core/Encoding/Box.h"

namespace pd {

class Resource {
public:
    static ResourceType GetType() { return ResourceType::Unknown; }
    
    virtual ~Resource() = default;

    virtual bool Load(StringView path) = 0;
    virtual bool Load(Box& box, StringView name) = 0;

    virtual u64 GetHash() { return hash; }

protected:
    u64 hash = 0;
};

}
