#pragma once
#include <FuncOrderDefinition.h>
#include <Internal/ISystem.h>
#include <Singleton.h>
#include <Window.h>

struct ImVec2;
struct Vector2;

namespace Glib
{
    enum class LogLevel;
}

namespace Glib::Internal::Debug
{
    using LogLevel = Glib::LogLevel;

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
        public Function::EndDrawOrderSet<-1>,
        public IWindowMessage
    {
        friend WeakPtr<ImGuiManager> SingletonPtr<ImGuiManager>::Instance();
        ImGuiManager() = default;

    public:
        bool Initialize();
        void BeginDraw();
        void DebugDraw();
        void EndDraw();
        void Finalize();

    public:
        void Log(std::string_view message, LogLevel loglevel);
        void SetRenderTarget() const;
        bool static WindowActive(std::string_view windowName);

    private:
        void DrawConsole();
        void CreateDock();
        void DrawGameView();
        void ResetLayout();
        void SetGUIStyle();
        void operator()(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) override;
    };
}
