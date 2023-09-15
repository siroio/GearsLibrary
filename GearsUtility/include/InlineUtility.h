#pragma once

namespace Glib
{
    /**
     * @brief ’l‚ğíœŒã•Ï”‚ğnullptr‚É‚·‚é
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
     * @brief ’l‚ğíœŒã•Ï”‚ğnullptr‚É‚·‚é
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
     * @brief ReleaseŠÖ”‚ğŒÄo‚µŒã•Ï”‚ğnullptr‚É‚·‚é
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
