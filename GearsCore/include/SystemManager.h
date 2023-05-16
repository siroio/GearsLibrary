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

namespace
{
    using namespace GLib::Utility;
    using namespace GLib::Internal;
    template<class T>
    concept HasSingletonSystem = std::derived_from<T, Interface::ISystem>&&
        std::derived_from<T, SingletonPtr<T>>;
}

class SystemManager : public Singleton<SystemManager>
{
    using FunctionVariant = std::variant<
        std::shared_ptr<Interface::IOrderFunc<void>>,
        std::shared_ptr<Interface::IOrderFunc<bool>>>;

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

    template<class T> requires HasInitializeFunc<T, bool>
    void AddInitialize(const WeakPtr<T>& instance);
    void AddInitialize(...)
    {}

    template<class T> requires HasUpdateFunc<T>
    void AddUpdate(const WeakPtr<T>& instance);
    void AddUpdate(...)
    {}

    template<class T> requires HasBeginDrawFunc<T>
    void AddBeginDraw(const WeakPtr<T>& instance);
    void AddBeginDraw(...)
    {}

    template<class T> requires HasDrawFunc<T>
    void AddDraw(const WeakPtr<T>& instance);
    void AddDraw(...)
    {}

    template<class T> requires HasDebugDrawFunc<T>
    void AddDebugDraw(const WeakPtr<T>& instance);
    void AddDebugDraw(...)
    {}

    template<class T> requires HasEndDrawFunc<T>
    void AddEndDraw(const WeakPtr<T>& instance);
    void AddEndDraw(...)
    {}

    template<class T> requires HasFinalizeFunc<T>
    void AddFinalize(const WeakPtr<T>& instance);
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

template<class T> requires HasInitializeFunc<T, bool>
inline void SystemManager::AddInitialize(const WeakPtr<T>& instance)
{
    systemFunctions[FunctionType::Initialize].push_back(std::make_shared<Function::HasInitializeObject<T, bool>>(instance));
}

template<class T> requires HasUpdateFunc<T>
inline void SystemManager::AddUpdate(const WeakPtr<T>& instance)
{
    systemFunctions[FunctionType::Update].push_back(std::make_shared<Function::HasUpdateObject<T, void>>(instance));
}

template<class T> requires HasBeginDrawFunc<T>
inline void SystemManager::AddBeginDraw(const WeakPtr<T>& instance)
{
    systemFunctions[FunctionType::BeginDraw].push_back(std::make_shared<Function::HasBeginDrawObject<T, void>>(instance));
}

template<class T> requires HasDrawFunc<T>
inline void SystemManager::AddDraw(const WeakPtr<T>& instance)
{
    systemFunctions[FunctionType::Draw].push_back(std::make_shared<Function::HasDrawObject<T, void>>(instance));
}

template<class T> requires HasDebugDrawFunc<T>
inline void SystemManager::AddDebugDraw(const WeakPtr<T>& instance)
{
    systemFunctions[FunctionType::DebugDraw].push_back(std::make_shared<Function::HasDebugDrawObject<T, void>>(instance));
}

template<class T> requires HasEndDrawFunc<T>
inline void SystemManager::AddEndDraw(const WeakPtr<T>& instance)
{
    systemFunctions[FunctionType::EndDraw].push_back(std::make_shared<Function::HasEndDrawObject<T, void>>(instance));
}

template<class T> requires HasFinalizeFunc<T>
inline void SystemManager::AddFinalize(const WeakPtr<T>& instance)
{
    systemFunctions[FunctionType::Finalize].push_back(std::make_shared<Function::HasFinalizeObject<T, void>>(instance));
}

#endif // !GEARS_SYSTEMMNAGER_H