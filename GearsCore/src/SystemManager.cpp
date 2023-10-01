#include <SystemManager.h>
#include <algorithm>
#include <ranges>
#include <string>

bool SystemManager::Initialize()
{
    auto& initilizeFunctions = systemFunctions_[SystemFunctionType::Initialize];
    std::ranges::sort(initilizeFunctions, [](const FunctionVariant& lhs, const FunctionVariant& rhs)
    {
        return std::get<1>(lhs)->Order() < std::get<1>(rhs)->Order();
    });

    int i = 0;
    for (const auto& initFunc : systemFunctions_[SystemFunctionType::Initialize])
    {
        if (initFunc.index() != 1) continue;
        i++;
        puts(std::to_string(i).data());
        const auto& func = std::get<1>(initFunc);
        if (!func->Call()) return false;
    }

    systemFunctions_.erase(SystemFunctionType::Initialize);

    for (auto&& [type, funcList] : systemFunctions_)
    {
        std::ranges::sort(funcList, [](const FunctionVariant& lhs, const FunctionVariant& rhs)
        {
            return std::get<0>(lhs)->Order() < std::get<0>(rhs)->Order();
        });
    }

    return true;
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
