#pragma once
namespace Glib::Internal
{
    class MemoryCheck
    {
    public:
#if defined(DEBUG) || defined(_DEBUG)
        /**
         * @brief メモリリークのチェックを開始
        */
        static void Start();

    private:
        static inline bool isStart{ false };

#else
        void Start();
#endif
    };
    }
