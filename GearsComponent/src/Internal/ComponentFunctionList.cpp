#include <Internal/ComponentFunctionList.h>
#include <Component.h>

class CameraBase;

void GLib::Internal::ComponentFunctionList::Update()
{
    for (auto it = addedFunction_.begin(); it != addedFunction_.end();)
    {
        // 期限を確認
        bool expired = std::visit([](const auto& v)
        {
            return v.component.expired();
        }, it->second);
        if (expired)
        {
            it = addedFunction_.erase(it);
            return;
        }

        // アクティブを確認
        bool active = std::visit([](const auto& v)
        {
            return v.component->Active();
        }, it->second);
        if (active)
        {
            functions_[it->first].push_back(it->second);
            it = addedFunction_.erase(it);
            return;
        }
        ++it;
    }
}

void GLib::Internal::ComponentFunctionList::Clear()
{
    addedFunction_.clear();
    functions_.clear();
}

void GLib::Internal::ComponentFunctionList::Execute(FunctionType type)
{
    ExecuteFromVariant<0>(type);
}

void GLib::Internal::ComponentFunctionList::Execute(FunctionType type, const GLib::WeakPtr<CameraBase>& camera)
{
    ExecuteFromVariant<1>(type, camera);
}

void GLib::Internal::ComponentFunctionList::ExecuteClear(FunctionType type)
{
    Execute(type);
    functions_.clear();
}

void GLib::Internal::ComponentFunctionList::ExecuteClear(FunctionType type, const GLib::WeakPtr<CameraBase>& camera)
{
    Execute(type, camera);
    functions_.clear();
}

void GLib::Internal::ComponentFunctionList::Remove(FunctionType type)
{
    std::erase_if(functions_[type], [](const auto& variant)
    {
        return std::visit([&variant](auto& v)
        {
            return v.function->IsDelete();
        }, variant);
    });
}
