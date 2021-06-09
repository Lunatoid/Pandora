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

    /**
     * \return The global resource catalog.
     */
    static ResourceCatalog& Get();

    /**
     * \brief Sets the .box file to load the assets from.
     * 
     * \param boxPath The path to the .box file.
     * \return Whether or not it loaded successfully.
     */
    bool Load(StringView boxPath);

    /**
     * \brief Loads the .box file from a config file.
     * 
     * \param configPath The path the config file.
     * \return Whether or not it loaded successfully.
     */
    bool LoadFromConfig(StringView configPath);

    /**
     * \brief Calls delete on the box data.
     */
    void Delete();

    /**
     * \brief Changes the reference the catalog has to a weak reference.
     * If `forceFree` is true, then it will force delete the reference.
     * This can have undesired side effects!
     * 
     * Caution! `DeleteResource()` should NOT be used in combination
     * with `Sweep()`. `Sweep()` will delete any resource with 1 strong reference,
     * so if you cal `DeleteResource()` first, that 1 strong reference will NOT be
     * the catalog, so you will likely encounter a crash.
     * 
     * \tparam T The resource type.
     * \param name The resource name.
     * \param forceFree Whether or not to forcefully free the resource.
     * Can have undesirable side effects.
     */
    template<typename T>
    void DeleteResource(StringView name, bool forceFree = false);

    /**
     * \brief Changes the reference the catalog has to a weak reference.
     * If `forceFree` is true, then it will force delete the reference.
     * This can have undesired side effects!
     * 
     * Caution! `DeleteResource()` should NOT be used in combination
     * with `Sweep()`. `Sweep()` will delete any resource with 1 strong reference,
     * so if you cal `DeleteResource()` first, that 1 strong reference will NOT be
     * the catalog, so you will likely encounter a crash.
     * 
     * \param type The resource type.
     * \param name The resource name.
     * \param forceFree Whether or not to forcefully free the resource.
     * Can have undesirable side effects.
     */
    void DeleteResource(ResourceType type, StringView name, bool forceFree = false);

    /**
     * \brief Deletes any resource that only has 1 strong reference.
     * 
     * \return How many resources got swept.
     */
    int Sweep();

    /**
     * \brief Gets a resource from the box.
     * 
     * \tparam T The resource type.
     * \param name The resource name.
     * \return A new reference to the resource.
     */
    template<typename T>
    Ref<T> Get(StringView name);

    /**
     * \brief Gets the uncompressed data of the specified resource.
     * 
     * \param name The resource name.
     * \param out The output array.
     * \return Whether or not it succeeded.
     */
    bool GetResourceData(StringView name, Array<byte>& out);

    /**
     * \brief Installs the handler to handle loading the resouces.
     * 
     * \param type The resource type.
     * \param handler The handler function.
     * \param data Custom data to pass to the handler function.
     */
    void SetResourceRequestHandler(ResourceType type, OnRequestResource* handler,
                                   void* data = nullptr);

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
