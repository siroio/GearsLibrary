#include "SystemManager.h"
#include <algorithm>

bool SystemManager::Initialize()
{
    auto& initilizeFunctions = systemFunctions[FunctionType::Initialize];
    std::sort(initilizeFunctions.begin(), initilizeFunctions.end(), [](const FunctionVariant& lhs, const FunctionVariant& rhs)
    {
        return std::get<1>(lhs)->Order() < std::get<1>(rhs)->Order();
    });

    for (const auto& initFunc : systemFunctions[FunctionType::Initialize])
    {
        const auto& func = std::get<1>(initFunc);
        if (!func) return false;
        func->Call();
    }

    systemFunctions.erase(FunctionType::Initialize);

    for (auto& [type, funcList] : systemFunctions)
    {
        std::sort(funcList.begin(), funcList.end(), [](const FunctionVariant& lhs, const FunctionVariant& rhs)
        {
            return std::get<0>(lhs)->Order() < std::get<0>(rhs)->Order();
        });
    }

    return true;
}

void SystemManager::Update()
{
    Execute(FunctionType::Update);
}

void SystemManager::BeginDraw()
{
    Execute(FunctionType::BeginDraw);
}

void SystemManager::Draw()
{
    Execute(FunctionType::Draw);
}

void SystemManager::DebugDraw()
{
    Execute(FunctionType::DebugDraw);
}

void SystemManager::EndDraw()
{
    Execute(FunctionType::EndDraw);
}

void SystemManager::Finalize()
{
    Execute(FunctionType::Finalize);
}

void SystemManager::Execute(FunctionType type)
{
    for (const auto& func : systemFunctions[type])
    {
        if (func.index() != 0) continue;
        std::get<0>(func)->Call();
    }
}
