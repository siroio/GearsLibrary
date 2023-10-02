#include <iostream>
#include <format>
#include <string_view>
#include <TimeUtility.h>

namespace Glib
{
    /**
     * @brief �ȈՃ��K�[�N���X
     * @brief �t�@�C���o�͂Ȃ�
     */
    class Debug final
    {
    private:
        Debug() = default;
        Debug(const Debug&) = delete;
        Debug& operator = (const Debug&) = delete;

    public:
        enum class LogLevel
        {
            Info,
            Warn,
            Error
        };

        static bool Enabled();

        /**
         * @brief ���b�Z�[�W�w��Ȃ�assert
         */
        static void Assert(bool expression);

        /**
         * @brief ���b�Z�[�W�w�肠��assert
         */
        static void Assert(bool expression, std::string_view message);

        /**
         * @brief ���b�Z�[�W�o��
         */
        static void Log(std::string_view message, LogLevel loglevel = LogLevel::Info);

        /**
         * @brief �G���[���b�Z�[�W�o��
         * @param message 
        */
        static void Error(std::string_view message);

        /**
         * @brief �x�����b�Z�[�W�o��
         * @param message 
        */
        static void Warn(std::string_view message);

#if defined(DEBUG) || defined(_DEBUG)
        /**
         * @brief �t�H�[�}�b�g�w��ŏo��
         */
        template<class... Args>
        static void Format(std::string_view fmt, const Args&... args)
        {
            std::cout << Glib::TimeUtility::CurrentTimeStr();
            std::cout << std::format(fmt, args) << std::endl;
        }
#endif
    };
}