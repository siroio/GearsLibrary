#pragma once
#include <FuncOrderDefinition.h>
#include <Internal/ISystem.h>
#include <Singleton.h>

struct ImVec2;
struct Vector2;
struct ID3D12DescriptorHeap;

namespace Glib::Internal::Debug
{
    /**
     * @brief ログレベル
     */
    enum class LogLevel
    {
        INFO,
        WARN,
        ERROR
    };

    /**
     * @brief ログ保存用構造体
     */
    struct LogMessage
    {
        LogLevel level;
        std::string message;
    };

    class ImGuiManager :
        public Interface::ISystem,
        public SingletonPtr<ImGuiManager>,
        public Function::DebugDrawOrderSet<-1>,
        public Function::EndDrawOrderSet<-1>
    {
    public:
        bool Initialize();
        void BeginDraw();
        void DebugDraw();
        void EndDraw();
        void Finalize();

    public:
        void Log(const std::string message, LogLevel level);
        void RenderTarget() const;
        bool static WindowActive(const std::string& name);

    private:
        void CreateConsole();
        void CreateDock();
        void CreateGameScreen();
        void ResetLayout();
        void StyleColor();
    };

    static ImVec2 ToImVec2(const Vector2& v);
    static Vector2 ToVector2(const ImVec2& v);
}
