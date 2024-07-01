#include <Internal/ComponentFunctionList.h>
#include <Internal/CameraBase.h>
#include <Component.h>
#include <algorithm>

void Glib::Internal::ComponentFunctionList::Update()
{
    std::erase_if(addedFunction_, [&](auto& pair)
    {
        // 期限を確認
        if (std::visit([](const auto& v)
        {
            return v.component.expired();
        }, pair.second))
        {
            return true; // 削除
        }

        // アクティブを確認
        if (std::visit([](const auto& v)
        {
            return v.component->Active();
        }, pair.second))
        {
            functions_[pair.first].push_back(pair.second);
            return true; // 削除
        }

        return false; // 保持
    });
}

void Glib::Internal::ComponentFunctionList::Clear()
{
    addedFunction_.clear();
    functions_.clear();
}

void Glib::Internal::ComponentFunctionList::Execute(FunctionType type)
{
    Remove(type);
    ExecuteFunction<0>(type);
}

void Glib::Internal::ComponentFunctionList::Execute(FunctionType type, const Glib::WeakPtr<CameraBase>& camera)
{
    Remove(type);
    ExecuteFunction<1>(type, camera);
}

void Glib::Internal::ComponentFunctionList::ExecuteClear(FunctionType type)
{
    Execute(type);
    functions_[type].clear();
}

void Glib::Internal::ComponentFunctionList::ExecuteClear(FunctionType type, const Glib::WeakPtr<CameraBase>& camera)
{
    Execute(type, camera);
    functions_[type].clear();
}

void Glib::Internal::ComponentFunctionList::Remove(FunctionType type)
{
    auto result = std::ranges::remove_if(functions_[type], [](const FunctionVariant& variant)
    {
        return std::visit([](auto& v)
        {
            return v.function->IsDelete();
        }, variant);
    });

    functions_[type].erase(result.begin(), result.end());
}
