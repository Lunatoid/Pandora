#include "ResourceCatalog.h"

#include "Pandora/Core/IO/Console.h"

#include "Pandora/Core/Resources/BinaryResource.h"

namespace pd {

ResourceCatalog::ResourceCatalog() {
    catalogs.Reserve((int)ResourceType::Count);

    // Initialize all handlers to stubs
    for (int i = 0; i < (int)ResourceType::Count; i++) {
        SetResourceRequestHandler((ResourceType)i, [](Box& box, ResourceType type, StringView name, void* data) -> Resource* {
            CONSOLE_LOG_DEBUG("[{}Catalog Error{}] no request handler set for resource type {}\n", ConColor::Red, ConColor::White, type);
            return nullptr;
        }, nullptr);
    }

    // Set binary request handler
    SetResourceRequestHandler(ResourceType::Binary, [](Box& box, ResourceType type, StringView name, void* data) {
        BinaryResource* binary = New<BinaryResource>();

        if (!binary->Load(box, name)) {
            CONSOLE_LOG_DEBUG("[{}Resource Error{}] could not load binary resource {#} from box\n", ConColor::Red, ConColor::White, name);
        }

        return (Resource*)binary;
    });
}

ResourceCatalog::~ResourceCatalog() {
    // The hackiest of @HACK's
    // Since we're explicitly calling the
    // destructor it will get called again
    // on static destruction
    if (catalogs.Count() == 0) return;

    Delete();

#if defined(PD_DEBUG)
    Sweep();
    for (int i = 0; i < (int)ResourceType::Count; i++) {
        TypeCatalog& catalog = catalogs[i];

        for (const auto& rsc : catalog) {
            console.Log("[{}Resource Error{}] resource '{}' has {} strong references at the end of it's lifecycle\n",
                ConColor::Red, ConColor::White, rsc.key, rsc.val.Count(RefType::Strong));
        }
    }
#endif
}

bool ResourceCatalog::Load(StringView boxPath) {
    return box.Load(boxPath);
}

bool ResourceCatalog::LoadFromConfig(StringView configPath) {
    return box.LoadFromConfig(configPath);
}

void ResourceCatalog::Delete() {
    box.Delete();
}

void ResourceCatalog::DeleteResource(ResourceType type, StringView name, bool forceFree) {
    TypeCatalog* catalog = &catalogs[(int)type];

    String nameStr = name.ToString();
    Ref<Resource>& rscPtr = catalog->Get(&nameStr);

    if (forceFree) {
        rscPtr.Delete();
    } else {
        rscPtr.ChangeType(RefType::Weak);
    }

    if (!rscPtr) {
        catalog->Remove(&name);
    }
}

int ResourceCatalog::Sweep() {
    int removeCount = 0;
    for (int typeI = 0; typeI < (int)ResourceType::Count; typeI++) {
        ResourceType type = (ResourceType)typeI;
        TypeCatalog& catalog = catalogs[typeI];

        Array<String> toDelete;
        for (const auto& rsc : catalog) {
            // If it has only 1 strong reference then that's probably us
            if (rsc.val.Count(RefType::Strong) == 1) {
                toDelete.Add(rsc.key);
            }
        }

        for (const auto& key : toDelete) {
            catalog.Remove(key);
        }

        removeCount += toDelete.Count();
    }

    return removeCount;
}

bool ResourceCatalog::GetResourceData(StringView name, Array<byte>& out) {
    return box.GetResourceData(name, out);
}

void ResourceCatalog::SetResourceRequestHandler(ResourceType type, OnRequestResource* handler, void* data) {
    onRequestResource[(int)type] = handler;
    requestUserData[(int)type] = data;
}

ResourceCatalog& ResourceCatalog::Get() {
    // @GLOBAL
    static ResourceCatalog catalog;
    return catalog;
}

}
