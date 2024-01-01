#pragma once

namespace Glib
{
    /**
     * @brief 値を削除後変数をnullptrにする
     */
    template<class T>
    inline void SafeDelete(T*& ptr)
    {
        if (ptr != nullptr)
        {
            delete ptr;
            ptr = nullptr;
        }
    }

    /**
     * @brief 値を削除後変数をnullptrにする
     */
    template<class T>
    inline void SafeDeleteArray(T*& ptr)
    {
        if (ptr != nullptr)
        {
            delete[] ptr;
            ptr = nullptr;
        }
    }

    /**
     * @brief Release関数を呼出し後変数をnullptrにする
     */
    template<class T>
    inline void SafeRelease(T*& ptr)
    {
        if constexpr (requires{ ptr->Release(); })
        {
            if (ptr == nullptr) return;
            ptr->Release();
            ptr = nullptr;
        }
    }
}

