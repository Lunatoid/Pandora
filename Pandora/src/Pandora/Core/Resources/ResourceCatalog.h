#pragma once

#include "Pandora/Core/Data/Dictionary.h"
#include "Pandora/Core/Data/Array.h"
#include "Pandora/Core/Data/Reference.h"

#include "Pandora/Core/Resources/Resource.h"

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

    /// <summary>
    /// Sets the resource box to Load all the assets from.
    /// </summary>
    /// <param name="boxPath">The path to the .box file.</param>
    /// <returns>Whether or not it loaded successfully.</returns>
    bool Load(StringView boxPath);

    /// <summary>
    /// Loads the assets from the filesystem using a config file.
    /// This is the same config file that is used to build a .box file.
    /// </summary>
    /// <param name="configPath">The path to the config file.</param>
    /// <returns>Whether or not it loaded successfully.</returns>
    bool LoadFromConfig(StringView configPath);

    /// <summary>
    /// Deletes the box/config data.
    /// </summary>
    void Delete();

    /// <summary>
    /// Changes the reference the catalog has to a weak reference.
    /// If <c>forceFree</c> is <c>true</c> then it will force delete
    /// the reference. This can have undesired consequences.
    ///
    /// Caution! DeleteResource() should not be used in combination with Sweep().
    /// Sweep() will delete any resources with 1 strong reference, so if you call
    /// DeleteResource() first then that 1 strong reference will NOT be the catalog
    /// but something else.
    /// </summary>
    /// <param name="name">The name of the resource.</param>
    /// <param name="forceFree">Whether or not to forcefully free the resource.</param> 
    template<typename T>
    void DeleteResource(StringView name, bool forceFree = false);

    /// <summary>
    /// Changes the reference the catalog has to a weak reference.
    /// If <c>forceFree</c> is <c>true</c> then it will force delete
    /// the reference. This can have undesired consequences.
    /// </summary>
    /// <param name="type">The type of the resource.</param>
    /// <param name="name">The name of the resource.</param>
    /// <param name="forceFree">Whether or not to forcefully free the resource.</param> 
    void DeleteResource(ResourceType type, StringView name, bool forceFree = false);

    /// <summary>
    /// Deletes all resources that have only 1 strong reference.
    /// </summary>
    /// <returns>How many resources got swept.</returns>
    int Sweep();

    template<typename T>
    Ref<T> Get(StringView name);

    bool GetResourceData(StringView name, Array<byte>& out);

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

    // Use handler to Load it if there is no data in this reference
    if (!rsc.Get()) {
        rsc.Reset(onRequestResource[(int)type](box, type, name, requestUserData[(int)type]));
    }

    // Cast to appropiate type
    return *((Ref<T>*)&rsc);
}

}
