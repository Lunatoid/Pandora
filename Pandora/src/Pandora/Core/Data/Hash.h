#pragma once

#include "Pandora/Core/Types.h"
#include "Pandora/Core/Data/String.h"

#include "Pandora/Libs/MeowHash/meow_hash_x64_aesni.h"

namespace pd {

/**
 * \brief Hashes the data.
 * 
 * \tparam T The object type.
 * \param data A pointer to the object to hash.
 * \return The hash.
 */
template<typename T>
inline u64 DoHash(const T& data) {
    return MeowU64From(MeowHash(MeowDefaultSeed, sizeof(T), (void*)&data), 0);
}

/**
 * \brief Hashes a buffer.
 * 
 * \tparam T The type of the buffer.
 * \param buffer A pointer to the buffer.
 * \param bufferSize The size of the buffer in bytes.
 * \return The hash.
 */
template<typename T>
inline u64 DoHash(T* buffer, u64 bufferSize) {
    return MeowU64From(MeowHash(MeowDefaultSeed, bufferSize, (void*)buffer), 0);
}

/**
 * \brief Hashes the slice.
 * 
 * \tparam T The type of the slice.
 * \param data The slice.
 * \return The hash.
 */
template<typename T>
inline u64 DoHash(Slice<T> data) {
    return DoHash(data.Data(), data.SizeInBytes());
}
//
// String hashing
//

/**
 * \brief Hashes the string.
 * 
 * \param data The string.
 * \return The hash.
 */
template<>
inline u64 DoHash(const String& data) {
    return MeowU64From(MeowHash(MeowDefaultSeed, data.SizeInBytes(), data.Data()), 0);
}

/**
 * \brief Hashes the bounded string.
 * 
 * \tparam cap The capacity of the string.
 * \param data The string.
 * \return The hash.
 */
template<int cap>
inline u64 DoHash(const BoundedString<cap>& data) {
    return DoHash<String>(data);
}

/**
 * \brief Hashes the string view.
 * 
 * \param data The view.
 * \return The hash.
 */
template<>
inline u64 DoHash(const StringView& data) {
    return MeowU64From(MeowHash(MeowDefaultSeed, data.SizeInBytes(), (void*)data.Data()), 0);
}

}
