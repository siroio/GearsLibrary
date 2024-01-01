#pragma once
#include <FuncOrderDefinition.h>
#include <Internal/ISystem.h>
#include <Singleton.h>
#include <Debugger.h>

struct ImVec2;
struct Vector2;

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
        public Function::EndDrawOrderSet<-1>
    {
    public:
        bool Initialize();
        void BeginDraw();
        void DebugDraw();
        void EndDraw();
        void Finalize();

    public:
        void Log(std::string_view message, LogLevel loglevel = LogLevel::Info);
        void SetRenderTarget() const;
        bool static WindowActive(std::string_view windowName);

    private:
        void DrawConsole();
        void CreateDock();
        void DrawGameView();
        void ResetLayout();
        void SetGUIStyle();
    };
}

