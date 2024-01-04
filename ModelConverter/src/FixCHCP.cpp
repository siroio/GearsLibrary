#include <windows.h>
#include <cstdio>
#include <exception>

namespace AUTOUTF8
{
    struct CHCP_UTF8
    {
        static inline UINT old_ = 0;

        CHCP_UTF8() noexcept
        {
            UINT cp = GetConsoleOutputCP();
            if (cp != 0 && cp != CP_UTF8)
            {
                old_ = cp;
                SetConsoleOutputCP(CP_UTF8);
                std::set_terminate(Restore);
            }
        }

        ~CHCP_UTF8() noexcept
        {
            Restore();
        }

        static void Restore() noexcept
        {
            UINT cp = old_;
            if (cp != 0 && cp != CP_UTF8)
            {
                SetConsoleOutputCP(cp);
                old_ = 0;
            }
        }
    };
}

namespace
{
    static AUTOUTF8::CHCP_UTF8 automatic{};
}
