#include <SystemManager.h>
#include <algorithm>
#include <ranges>
#include <Window.h>

bool SystemManager::Initialize()
{
    auto& initilizeFunctions = systemFunctions_[SystemFunctionType::Initialize];
    std::ranges::sort(initilizeFunctions, [](const FunctionVariant& lhs, const FunctionVariant& rhs)
    {
        return std::get<1>(lhs)->Order() < std::get<1>(rhs)->Order();
    });

    for (const auto& initFunc : systemFunctions_[SystemFunctionType::Initialize])
    {
        if (initFunc.index() != 1) continue;
        const auto& func = std::get<1>(initFunc);
        if (!func->Call()) return false;
    }

    systemFunctions_.erase(SystemFunctionType::Initialize);
    std::ranges::reverse(systemFunctions_.at(SystemFunctionType::Finalize));

    for (auto&& funcList : systemFunctions_ | std::ranges::views::values)
    {
        std::ranges::sort(funcList, [](const FunctionVariant& lhs, const FunctionVariant& rhs)
        {
            return std::get<0>(lhs)->Order() < std::get<0>(rhs)->Order();
        });
    }

    return true;
}

void SystemManager::FixedUpdate()
{
    Execute(SystemFunctionType::FixedUpdate);
}

void SystemManager::Update()
{
    Execute(SystemFunctionType::Update);
}

void SystemManager::BeginDraw()
{
    Execute(SystemFunctionType::BeginDraw);
}

void SystemManager::Draw()
{
    Execute(SystemFunctionType::Draw);
}

void SystemManager::DebugDraw()
{
    Execute(SystemFunctionType::DebugDraw);
}

void SystemManager::EndDraw()
{
    Execute(SystemFunctionType::EndDraw);
}

void SystemManager::Finalize()
{
    Execute(SystemFunctionType::Finalize);
}

void SystemManager::Execute(SystemFunctionType type)
{
    for (const auto& func : systemFunctions_[type])
    {
        if (func.index() != 0) continue;
        std::get<0>(func)->Call();
    }
}
