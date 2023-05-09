#include "SystemManager.h"
#include <algorithm>

bool SystemManager::Initialize()
{
    auto& initilizeFunctions = systemFunctions[FunctionType::Initilialize];
    std::sort(initilizeFunctions.begin(), initilizeFunctions.end(), [](const FunctionVariant& lhs, const FunctionVariant& rhs)
    {
        return std::get<1>(lhs)->Order() < std::get<1>(rhs)->Order();
    });

    for (const auto& init : systemFunctions[FunctionType::Initilialize])
    {
        if (!std::get<1>(init)->Call()) return false;
    }

    systemFunctions.erase(FunctionType::Initilialize);

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
    CallFunc(FunctionType::Update);
}

void SystemManager::BeginDraw()
{
    CallFunc(FunctionType::BeginDraw);
}

void SystemManager::Draw()
{
    CallFunc(FunctionType::Draw);
}

void SystemManager::DebugDraw()
{
    CallFunc(FunctionType::DrawDebug);
}

void SystemManager::EndDraw()
{
    CallFunc(FunctionType::EndDraw);
}

void SystemManager::Finalize()
{
    CallFunc(FunctionType::Finalize);
}

void SystemManager::CallFunc(FunctionType type)
{
    for (const auto& func : systemFunctions[type])
    {
        if (func.index() == 0) return;
        std::get<0>(func)->Call();
    }
}
