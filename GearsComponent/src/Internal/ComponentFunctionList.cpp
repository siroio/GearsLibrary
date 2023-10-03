#include <Internal/ComponentFunctionList.h>
#include <Internal/CameraBase.h>
#include <Component.h>

void Glib::Internal::ComponentFunctionList::Update()
{
    for (auto it = addedFunction_.begin(); it != addedFunction_.end();)
    {
        // 期限を確認
        if (std::visit([](const auto& v)
        {
            return v.component.expired();
        }, it->second))
        {
            it = addedFunction_.erase(it);
        }

        // アクティブを確認
        else if (std::visit([](const auto& v)
        {
            return v.component->Active();
        }, it->second))
        {
            functions_[it->first].push_back(it->second);
            it = addedFunction_.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Glib::Internal::ComponentFunctionList::Clear()
{
    addedFunction_.clear();
    functions_.clear();
}

void Glib::Internal::ComponentFunctionList::Execute(FunctionType type)
{
    ExecuteFromVariant<0>(type);
}

void Glib::Internal::ComponentFunctionList::Execute(FunctionType type, const Glib::WeakPtr<CameraBase>& camera)
{
    ExecuteFromVariant<1>(type, camera);
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
    std::erase_if(functions_[type], [](const auto& variant)
    {
        return std::visit([&variant](auto& v)
        {
            return v.function->IsDelete();
        }, variant);
    });
}
