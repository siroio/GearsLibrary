#ifndef GEARS_SYSTEMMANAGER_H
#define GEARS_SYSTEMMANAGER_H
#include <Singleton.h>
#include <Internal/IOrderFunc.h>
#include <unordered_map>
#include <vector>
#include <variant>
#include <memory>

class SystemManager : public Singleton<SystemManager>
{
    using FunctionVariant = std::variant<
        std::shared_ptr<GLib::Internal::Interface::IOrderFunc<void>>,
        std::shared_ptr<GLib::Internal::Interface::IOrderFunc<bool>>>;

    enum class FunctionType
    {
        Initilialize,
        Update,
        BeginDraw,
        Draw,
        DrawDebug,
        EndDraw,
        Finalize,
    };

public:
    bool Initialize();
    void Update();
    void BeginDraw();
    void Draw();
    void DebugDraw();
    void EndDraw();
    void Finalize();

private:
    void CallFunc(FunctionType type);

private:
    std::unordered_map<FunctionType, std::vector<FunctionVariant>> systemFunctions;
};

#endif // !GEARS_SYSTEMMNAGER_H
