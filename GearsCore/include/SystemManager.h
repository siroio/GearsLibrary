#ifndef GEARS_SYSTEMMANAGER_H
#define GEARS_SYSTEMMANAGER_H
#include <iostream>
#include <Singleton.h>
#include <Internal/IFunc.h>
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
    template<class T>
    static void AddSystem()
    {}

private:
    void Execute(FunctionType type);

    template<class T> requires GLib::Internal::HasInitializeFunc<T, bool>
    void AddInitialize(const GLib::Utility::WeakPtr<T>& instance);
    template<class T>
    void AddInitialize(...)
    {}

    template<class T> requires GLib::Internal::HasUpdateFunc<T>
    void AddUpdate(const GLib::Utility::WeakPtr<T>& instance);
    template<class T>
    void AddUpdate(...)
    {}

    template<class T> requires GLib::Internal::HasBeginDrawFunc<T>
    void AddBeginDraw(const GLib::Utility::WeakPtr<T>& instance);
    template<class T>
    void AddBeginDraw(...)
    {}

    template<class T> requires GLib::Internal::HasDrawFunc<T>
    void AddDraw(const GLib::Utility::WeakPtr<T>& instance);
    template<class T>
    void AddDraw(...)
    {}

    template<class T> requires GLib::Internal::HasDebugDrawFunc<T>
    void AddDebugDraw(const GLib::Utility::WeakPtr<T>& instance);
    template<class T>
    void AddDebugDraw(...)
    {}

    template<class T> requires GLib::Internal::HasEndDrawFunc<T>
    void AddEndDraw(const GLib::Utility::WeakPtr<T>& instance);
    template<class T>
    void AddEndDraw(...)
    {}

    template<class T> requires GLib::Internal::HasFinalizeFunc<T>
    void AddFinalize(const GLib::Utility::WeakPtr<T>& instance);
    template<class T>
    void AddFinalize(...)
    {}

private:
    std::unordered_map<FunctionType, std::vector<FunctionVariant>> systemFunctions_;
};

template<class T> requires HasSingletonSystem<T>
inline void SystemManager::AddSystem()
{
    GLib::Utility::WeakPtr<T> instance = T::Instance();
    Instance().AddInitialize<T>(instance);
    Instance().AddUpdate<T>(instance);
    Instance().AddBeginDraw<T>(instance);
    Instance().AddDraw<T>(instance);
    Instance().AddDebugDraw<T>(instance);
    Instance().AddEndDraw<T>(instance);
    Instance().AddFinalize<T>(instance);
}

template<class T> requires GLib::Internal::HasInitializeFunc<T, bool>
inline void SystemManager::AddInitialize(const GLib::Utility::WeakPtr<T>& instance)
{
    systemFunctions_[FunctionType::Initialize].push_back(std::make_shared<GLib::Internal::Function::HasInitializeObject<T, bool>>(instance));
}

template<class T> requires GLib::Internal::HasUpdateFunc<T>
inline void SystemManager::AddUpdate(const GLib::Utility::WeakPtr<T>& instance)
{
    systemFunctions_[FunctionType::Update].push_back(std::make_shared<GLib::Internal::Function::HasUpdateObject<T, void>>(instance));
}

template<class T> requires GLib::Internal::HasBeginDrawFunc<T>
inline void SystemManager::AddBeginDraw(const GLib::Utility::WeakPtr<T>& instance)
{
    systemFunctions_[FunctionType::BeginDraw].push_back(std::make_shared<GLib::Internal::Function::HasBeginDrawObject<T, void>>(instance));
}

template<class T> requires GLib::Internal::HasDrawFunc<T>
inline void SystemManager::AddDraw(const GLib::Utility::WeakPtr<T>& instance)
{
    systemFunctions_[FunctionType::Draw].push_back(std::make_shared<GLib::Internal::Function::HasDrawObject<T, void>>(instance));
}

template<class T> requires GLib::Internal::HasDebugDrawFunc<T>
inline void SystemManager::AddDebugDraw(const GLib::Utility::WeakPtr<T>& instance)
{
    systemFunctions_[FunctionType::DebugDraw].push_back(std::make_shared<GLib::Internal::Function::HasDebugDrawObject<T, void>>(instance));
}

template<class T> requires GLib::Internal::HasEndDrawFunc<T>
inline void SystemManager::AddEndDraw(const GLib::Utility::WeakPtr<T>& instance)
{
    systemFunctions_[FunctionType::EndDraw].push_back(std::make_shared<GLib::Internal::Function::HasEndDrawObject<T, void>>(instance));
}

template<class T> requires GLib::Internal::HasFinalizeFunc<T>
inline void SystemManager::AddFinalize(const GLib::Utility::WeakPtr<T>& instance)
{
    systemFunctions_[FunctionType::Finalize].push_back(std::make_shared<GLib::Internal::Function::HasFinalizeObject<T, void>>(instance));
}

#endif // !GEARS_SYSTEMMNAGER_H