#if !defined(PD_NO_DIRECTX)
#pragma once

namespace pd {

template<typename T>
class DXComRef {
public:
    DXComRef() = default;
    DXComRef(DXComRef<T>& other);
    DXComRef(T* data);

    ~DXComRef();

    void Reset(T* newData = nullptr);
    T*& Get();

    /// <summary>
    /// Same as <c>Get()</c> but calls <c>Reset()</c> and then sets the
    /// reference count to 1.
    /// </summary>
    /// <returns>A pointer as an out parameter.</returns>
    T*& AsOut();

    DXComRef<T>& operator=(DXComRef<T>& other);

    T*& operator->();

    T& operator*();

    operator bool();

private:
    void Copy(DXComRef<T>& other) {
        // If you do something like `ref = ref;` we don't want to increase the count
        // This can lead to things having an infinite lifespan. Bad!
        if (data == other.data) return;

        DecCount();

        data = other.data;
        count = other.count;

        IncCount();
    }

    inline void IncCount() {
        if (!count) return;

        count->strong += 1;
    }

    inline void DecCount() {
        if (!count) return;

        *count -= 1;

        if (*count <= 0) {
            PD_ASSERT_D(*count == 0, "negative reference count: %d", *count);
            Reset();
        }
    }

    T* data = nullptr;
    int* count = nullptr;
};

template<typename T>
inline DXComRef<T>::DXComRef(DXComRef<T>& other) {
    Copy(other);
}

template<typename T>
inline DXComRef<T>::DXComRef(T* data) {
    Reset(data);
}

template<typename T>
inline DXComRef<T>::~DXComRef() {
    DecCount();
}

template<typename T>
inline void DXComRef<T>::Reset(T* newData) {
    // Delete the old
    if (count && !newData) {
        Delete(count);
        count = nullptr;
    }

    if (data) {
        data->Release();
        data = nullptr;
    }

    // Rise out of the ashes
    if (newData) {
        if (!count) {
            count = New<int>(1);
        } else {
            *count = 1;
        }

        data = newData;
    }
}

template<typename T>
inline T*& DXComRef<T>::Get() {
    return data;
}

template<typename T>
inline T*& DXComRef<T>::AsOut() {
    Reset();

    if (!count) {
        count = New<int>(1);
    } else {
        *count = 1;
    }

    return data;
}

template<typename T>
inline DXComRef<T>& DXComRef<T>::operator=(DXComRef<T>& other) {
    Copy(other);
    return *this;
}

template<typename T>
inline T*& DXComRef<T>::operator->() {
    return Get();
}

template<typename T>
inline T& DXComRef<T>::operator*() {
    return *Get();
}

template<typename T>
inline DXComRef<T>::operator bool() {
    return data != nullptr;
}

}

#endif
