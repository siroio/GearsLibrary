#pragma once
namespace Glib::Internal
{
    class MemoryCheck
    {
    public:
        /**
         * @brief メモリリークのチェックを開始
        */
        static void Start();

    private:
        static inline bool isStart{ false };
    };
}
