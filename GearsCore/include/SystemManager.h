#ifndef GEARS_SYSTEMMANAGER_H
#define GEARS_SYSTEMMANAGER_H
#include <iostream>
#include <Singleton.h>
#include <Internal/IFunc.h>
#include <Internal/SystemFunctionType.h>
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
        std::shared_ptr<GLib::Internal::Interface::IFunc<void>>,
        std::shared_ptr<GLib::Internal::Interface::IFunc<bool>>>;

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
    template<class T>
    static void AddSystem()
    {}

private:
    void Execute(SystemFunctionType type);

    template<class T> requires GLib::Internal::HasInitializeFunc<T, bool>
    void AddInitialize(const GLib::Utility::WeakPtr<T>& instance);
    void AddInitialize(...)
    {}

    template<class T> requires GLib::Internal::HasUpdateFunc<T, void>
    void AddUpdate(const GLib::Utility::WeakPtr<T>& instance);
    void AddUpdate(...)
    {}

    template<class T> requires GLib::Internal::HasBeginDrawFunc<T, void>
    void AddBeginDraw(const GLib::Utility::WeakPtr<T>& instance);
    void AddBeginDraw(...)
    {}

    template<class T> requires GLib::Internal::HasDrawFunc<T, void>
    void AddDraw(const GLib::Utility::WeakPtr<T>& instance);
    void AddDraw(...)
    {}

    template<class T> requires GLib::Internal::HasDebugDrawFunc<T, void>
    void AddDebugDraw(const GLib::Utility::WeakPtr<T>& instance);
    void AddDebugDraw(...)
    {}

    template<class T> requires GLib::Internal::HasEndDrawFunc<T, void>
    void AddEndDraw(const GLib::Utility::WeakPtr<T>& instance);
    void AddEndDraw(...)
    {}

    template<class T> requires GLib::Internal::HasFinalizeFunc<T, void>
    void AddFinalize(const GLib::Utility::WeakPtr<T>& instance);
    void AddFinalize(...)
    {}

private:
    std::unordered_map<SystemFunctionType, std::vector<FunctionVariant>> systemFunctions_;
};

template<class T> requires HasSingletonSystem<T>
inline void SystemManager::AddSystem()
{
    GLib::Utility::WeakPtr<T> instance = T::Instance();
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
    systemFunctions_[SystemFunctionType::Initialize].push_back(std::make_shared<GLib::Internal::Function::HasInitializeObject<T, bool>>(instance));
}

template<class T> requires GLib::Internal::HasUpdateFunc<T, void>
inline void SystemManager::AddUpdate(const GLib::Utility::WeakPtr<T>& instance)
{
    systemFunctions_[SystemFunctionType::Update].push_back(std::make_shared<GLib::Internal::Function::HasUpdateObject<T, void>>(instance));
}

template<class T> requires GLib::Internal::HasBeginDrawFunc<T, void>
inline void SystemManager::AddBeginDraw(const GLib::Utility::WeakPtr<T>& instance)
{
    systemFunctions_[SystemFunctionType::BeginDraw].push_back(std::make_shared<GLib::Internal::Function::HasBeginDrawObject<T, void>>(instance));
}

template<class T> requires GLib::Internal::HasDrawFunc<T, void>
inline void SystemManager::AddDraw(const GLib::Utility::WeakPtr<T>& instance)
{
    systemFunctions_[SystemFunctionType::Draw].push_back(std::make_shared<GLib::Internal::Function::HasDrawObject<T, void>>(instance));
}

template<class T> requires GLib::Internal::HasDebugDrawFunc<T, void>
inline void SystemManager::AddDebugDraw(const GLib::Utility::WeakPtr<T>& instance)
{
    systemFunctions_[SystemFunctionType::DebugDraw].push_back(std::make_shared<GLib::Internal::Function::HasDebugDrawObject<T, void>>(instance));
}

template<class T> requires GLib::Internal::HasEndDrawFunc<T, void>
inline void SystemManager::AddEndDraw(const GLib::Utility::WeakPtr<T>& instance)
{
    systemFunctions_[SystemFunctionType::EndDraw].push_back(std::make_shared<GLib::Internal::Function::HasEndDrawObject<T, void>>(instance));
}

template<class T> requires GLib::Internal::HasFinalizeFunc<T, void>
inline void SystemManager::AddFinalize(const GLib::Utility::WeakPtr<T>& instance)
{
    systemFunctions_[SystemFunctionType::Finalize].push_back(std::make_shared<GLib::Internal::Function::HasFinalizeObject<T, void>>(instance));
}

#endif // !GEARS_SYSTEMMNAGER_H