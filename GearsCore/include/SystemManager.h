#ifndef GEARS_SYSTEMMANAGER_H
#define GEARS_SYSTEMMANAGER_H

#include <Singleton.h>
#include <Internal/IOrderFunc.h>
#include <FuncDefinition.h>
#include <unordered_map>
#include <vector>
#include <variant>
#include <memory>
#include <concepts>

namespace GLib::Internal::Interface
{
    class ISystem;
}

template<class T>
concept HasSingletonSystem = std::derived_from<T, GLib::Internal::Interface::ISystem>&&
std::derived_from<T, GLib::Utility::SingletonPtr<T>>;

class SystemManager : public GLib::Utility::Singleton<SystemManager>
{
    using FunctionVariant = std::variant<
        std::shared_ptr<GLib::Internal::Interface::IOrderFunc<void>>,
        std::shared_ptr<GLib::Internal::Interface::IOrderFunc<bool>>>;

    enum class FunctionType
    {
        Initialize,
        Update,
        BeginDraw,
        Draw,
        DebugDraw,
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

    template<class T> requires HasSingletonSystem<T>
    static void AddSystem();

private:
    void Execute(FunctionType type);

    template<class T> requires GLib::Internal::HasInitializeFunc<T, bool>
    void AddInitialize(const GLib::Utility::WeakPtr<T>& instance);
    void AddInitialize(...)
    {}

    template<class T> requires GLib::Internal::HasUpdateFunc<T>
    void AddUpdate(const GLib::Utility::WeakPtr<T>& instance);
    void AddUpdate(...)
    {}

    template<class T> requires GLib::Internal::HasBeginDrawFunc<T>
    void AddBeginDraw(const GLib::Utility::WeakPtr<T>& instance);
    void AddBeginDraw(...)
    {}

    template<class T> requires GLib::Internal::HasDrawFunc<T>
    void AddDraw(const GLib::Utility::WeakPtr<T>& instance);
    void AddDraw(...)
    {}

    template<class T> requires GLib::Internal::HasDebugDrawFunc<T>
    void AddDebugDraw(const GLib::Utility::WeakPtr<T>& instance);
    void AddDebugDraw(...)
    {}

    template<class T> requires GLib::Internal::HasEndDrawFunc<T>
    void AddEndDraw(const GLib::Utility::WeakPtr<T>& instance);
    void AddEndDraw(...)
    {}

    template<class T> requires GLib::Internal::HasFinalizeFunc<T>
    void AddFinalize(const GLib::Utility::WeakPtr<T>& instance);
    void AddFinalize(...)
    {}

private:
    std::unordered_map<FunctionType, std::vector<FunctionVariant>> systemFunctions;
};

template<class T> requires HasSingletonSystem<T>
inline void SystemManager::AddSystem()
{
    if constexpr (!HasSingletonSystem<T>) return;
    WeakPtr<T> instance = T::Instance();
    Instance().AddInitialize(instance);
    Instance().AddUpdate(instance);
    Instance().AddBeginDraw(instance);
    Instance().AddDraw(instance);
    Instance().AddDebugDraw(instance);
    Instance().AddEndDraw(instance);
    Instance().AddFinalize(instance);
}

template<class T> requires GLib::Internal::HasInitializeFunc<T, bool>
inline void SystemManager::AddInitialize(const GLib::Utility::WeakPtr<T>& instance)
{
    systemFunctions[FunctionType::Initialize].push_back(std::make_shared<Function::HasInitializeObject<T, bool>>(instance));
}

template<class T> requires GLib::Internal::HasUpdateFunc<T>
inline void SystemManager::AddUpdate(const GLib::Utility::WeakPtr<T>& instance)
{
    systemFunctions[FunctionType::Update].push_back(std::make_shared<Function::HasUpdateObject<T, void>>(instance));
}

template<class T> requires GLib::Internal::HasBeginDrawFunc<T>
inline void SystemManager::AddBeginDraw(const GLib::Utility::WeakPtr<T>& instance)
{
    systemFunctions[FunctionType::BeginDraw].push_back(std::make_shared<Function::HasBeginDrawObject<T, void>>(instance));
}

template<class T> requires GLib::Internal::HasDrawFunc<T>
inline void SystemManager::AddDraw(const GLib::Utility::WeakPtr<T>& instance)
{
    systemFunctions[FunctionType::Draw].push_back(std::make_shared<Function::HasDrawObject<T, void>>(instance));
}

template<class T> requires GLib::Internal::HasDebugDrawFunc<T>
inline void SystemManager::AddDebugDraw(const GLib::Utility::WeakPtr<T>& instance)
{
    systemFunctions[FunctionType::DebugDraw].push_back(std::make_shared<Function::HasDebugDrawObject<T, void>>(instance));
}

template<class T> requires GLib::Internal::HasEndDrawFunc<T>
inline void SystemManager::AddEndDraw(const GLib::Utility::WeakPtr<T>& instance)
{
    systemFunctions[FunctionType::EndDraw].push_back(std::make_shared<Function::HasEndDrawObject<T, void>>(instance));
}

template<class T> requires GLib::Internal::HasFinalizeFunc<T>
inline void SystemManager::AddFinalize(const GLib::Utility::WeakPtr<T>& instance)
{
    systemFunctions[FunctionType::Finalize].push_back(std::make_shared<Function::HasFinalizeObject<T, void>>(instance));
}

#endif // !GEARS_SYSTEMMNAGER_H