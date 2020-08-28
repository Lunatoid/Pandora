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
    Delete();
}

bool ResourceCatalog::Load(StringView boxPath) {
    return box.Load(boxPath);
}

void ResourceCatalog::Delete() {
    box.Delete();
}

void ResourceCatalog::DeleteResource(ResourceType type, StringView name, bool forceFree) {
    TypeCatalog* catalog = &catalogs[(int)type];

    String nameStr = name.ToString();
    Ref<Resource>& rscPtr = catalog->Get(&nameStr);
}

void ResourceCatalog::Sweep() {
    for (int typeI = 0; typeI < (int)ResourceType::Count; typeI++) {
        ResourceType type = (ResourceType)typeI;
        TypeCatalog* catalog = &catalogs[typeI];

        DictEntry<String, Ref<Resource>>* data = catalog->Data();
        if (!data) continue;

        for (int i = 0; i < catalog->Capacity(); i++) {
            if (data[i].dist < 0) continue;

            // If it has only 1 strong reference then that's probably us
            if (data->val.Count(RefType::Strong) <= 1) {
                catalog->Remove(data[i].key);
            }
        }
    }
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
