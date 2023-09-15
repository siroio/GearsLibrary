#pragma once

namespace Glib
{
    /**
     * @brief �l���폜��ϐ���nullptr�ɂ���
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
     * @brief �l���폜��ϐ���nullptr�ɂ���
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
     * @brief Release�֐����ďo����ϐ���nullptr�ɂ���
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
