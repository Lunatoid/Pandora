#pragma once

#include "Pandora/Core/Data/Dictionary.h"
#include "Pandora/Core/Data/Array.h"
#include "Pandora/Core/Data/Reference.h"

#include "Pandora/Core/Resources/Resource.h"

// @DEBUG
#include "Pandora/Graphics/Texture.h"

namespace pd {

using TypeCatalog = Dictionary<String, Ref<Resource>>;
using CatalogStorage = BoundedArray<TypeCatalog, (int)ResourceType::Count>;

typedef Resource*(OnRequestResource)(Box& box, ResourceType type, StringView name, void* data);

class ResourceCatalog {
public:
    ResourceCatalog();
    ~ResourceCatalog();
    
    /// <summary>
    /// Returns the global resource catalog.
    /// </summary>
    /// <returns>The global resource catalog.</returns>
    static ResourceCatalog& Get();

    bool Load(StringView boxPath);
    void Delete();

    template<typename T>
    void DeleteResource(StringView name, bool forceFree = false);

    void DeleteResource(ResourceType type, StringView name, bool forceFree = false);

    void Sweep();

    template<typename T>
    Ref<T> Get(StringView name);

    void SetResourceRequestHandler(ResourceType type, OnRequestResource* handler, void* data = nullptr);

private:
    Box box;

    OnRequestResource* onRequestResource[(int)ResourceType::Count];
    void* requestUserData[(int)ResourceType::Count];

    // We have one hash table per catalog
    CatalogStorage catalogs;
};

template<typename T>
inline void ResourceCatalog::DeleteResource(StringView name, bool forceFree) {
    DeleteResource(T::GetType(), name, forceFree);
}

template<typename T>
inline Ref<T> ResourceCatalog::Get(StringView name) {
    static_assert(std::is_base_of<Resource, T>::value, "Template type T must derive from pd::Resource");

    ResourceType type = T::GetType();
    TypeCatalog* catalog = &catalogs[(int)type];

    String nameStr = name.ToString();
    Ref<Resource>& rsc = catalog->Get(nameStr);

    // Use handler to load it if there is no data in this reference
    if (!rsc.Get()) {
        rsc.Reset(onRequestResource[(int)type](box, type, name, requestUserData[(int)type]));
    }

    // Cast to appropiate type
    return *((Ref<T>*) & rsc);
}

}
