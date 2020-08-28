#pragma once

#include "Pandora/Core/Types.h"
#include "Pandora/Core/Data/String.h"

#include "Pandora/Libs/MeowHash/meow_hash_x64_aesni.h"

namespace pd {

/// <summary>
/// Hashes the data.
/// </summary>
/// <param name="data">The data.</param>
/// <returns>The hash.</returns>
template<typename T>
inline u64 DoHash(T* data) {
    return MeowU64From(MeowHash(MeowDefaultSeed, sizeof(T), (void*)data), 0);
}

/// <summary>
/// Hashes the memory.
/// </summary>
/// <param name="memory">The memory.</param>
/// <param name="bufferSize">How many bytes long the memory is.</param>
/// <returns>The hash.</returns>
template<typename T>
inline u64 DoHash(T* buffer, u64 bufferSize) {
    return MeowU64From(MeowHash(MeowDefaultSeed, bufferSize, (void*)buffer), 0);
}

/// <summary>
/// Hashes the slice.
/// </summary>
/// <param name="data">The slice.</param>
/// <returns>The hash.</returns>
template<typename T>
inline u64 DoHash(Slice<T> data) {
    return DoHash(data.Data(), data.SizeInBytes());
}
//
// String hashing
//

/// <summary>
/// Hashes the string.
/// </summary>
/// <param name="data">The string.</param>
/// <returns>The hash.</returns>
template<>
inline u64 DoHash(String* data) {
    return MeowU64From(MeowHash(MeowDefaultSeed, data->SizeInBytes(), data->Data()), 0);
}

/// <summary>
/// Hashes the bounded string.
/// </summary>
/// <param name="data">The string.</param>
/// <returns>The hash.</returns>
template<int cap>
inline u64 DoHash(BoundedString<cap>* data) {
    return DoHash<String>(data);
}

/// <summary>
/// Hashes the view.
/// </summary>
/// <param name="data">The view.</param>
/// <returns>The hash.</returns>
template<>
inline u64 DoHash(StringView* data) {
    return MeowU64From(MeowHash(MeowDefaultSeed, data->SizeInBytes(), (void*)data->Data()), 0);
}

}
