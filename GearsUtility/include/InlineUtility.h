#pragma once

namespace Glib
{
    template<class T>
    void SafeDelete(T*& ptr)
    {
        if (ptr != nullptr)
        {
            delete ptr;
            ptr = nullptr;
        }
    }

    template<class T>
    void SafeDeleteArray(T*& ptr)
    {
        if (ptr != nullptr)
        {
            delete[] ptr;
            ptr = nullptr;
        }
    }

    template<class T>
    void SafeRelease(T*& ptr)
    {
        if constexpr (requires{ ptr->Release(); })
        {
            if (ptr == nullptr) return;
            ptr->Release();
            ptr = nullptr;
        }
    }
}
