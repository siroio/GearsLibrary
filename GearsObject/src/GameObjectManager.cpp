#include <GameObjectManager.h>
#include <Internal/IGameObject.h>
#include <Internal/ImGuiInc.h>
#include <GLGUI.h>
#include <GameObject.h>
#include <WeakPtr.h>
#include <algorithm>
#include <iterator>

namespace
{
    bool s_enableHierarchy{ true };
    bool s_enableInspector{ true };
    GameObjectPtr s_selectObject{ nullptr };
}

void Glib::GameObjectManager::Update()
{
    std::erase_if(gameObjects_, [](const std::shared_ptr<GameObject>& gameObject)
    {
        return gameObject->IsDead();
    });

    for (const auto& gameObject : gameObjects_)
    {
        gameObject->RemoveDeadComponents();
    }
}

void Glib::GameObjectManager::DebugDraw()
{
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Window"))
    {
        GLGUI::CheckBox("Hierarchy", &s_enableHierarchy);
        GLGUI::CheckBox("Inspector", &s_enableInspector);
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    if (s_enableHierarchy)
    {
        ImGui::Begin("Hierarchy", &s_enableHierarchy);

        for (const auto& go : gameObjects_)
        {
            // �p�����[�^�[�\��
            if (!go->IsRoot()) continue;
            DrawDebugParams(go);
        }

        ImGui::End();
    }

    if (s_enableInspector)
    {
        ImGui::Begin("Inspector", &s_enableInspector);

        if (!s_selectObject.expired())
        {
            // �A�N�e�B�u�t���O�̕\��
            bool active = s_selectObject->Active();
            if (ImGui::Checkbox("##Active", &active))
            {
                s_selectObject->Active(active);
            }
            ImGui::SameLine();
            ImGui::Text("Name");
            ImGui::SameLine();

            // ���O�̕\��
            std::string name = s_selectObject->Name();
            name.resize(64);
            if (ImGui::InputText("##Name", name.data(), name.size(), ImGuiInputTextFlags_AutoSelectAll))
            {
                s_selectObject->Name(name);
            }

            // ���C���[�̕\��
            std::string layer = "Layer: " + std::to_string(s_selectObject->Layer());
            ImGui::Text(layer.c_str());

            ImGui::Separator();

            // �R���|�[�l���g�̕\��
            s_selectObject->DrawGUI();
        }

        ImGui::End();
    }
}

void Glib::GameObjectManager::Finalize()
{
    gameObjects_.clear();
}

void Glib::GameObjectManager::ResetGameObjects()
{
    for (const auto& gameObject : gameObjects_)
    {
        if (gameObject->DontDestroyOnLoad()) continue;
        gameObject->Destroy();
    }
}

void Glib::GameObjectManager::DrawDebugParams(GameObjectPtr gameObject)
{
    ImGuiTreeNodeFlags flag{
        ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_OpenOnDoubleClick
    };

    if (gameObject->Transform()->Children().empty())
        flag |= ImGuiTreeNodeFlags_Leaf;

    if (gameObject == s_selectObject)
        flag |= ImGuiTreeNodeFlags_Selected;

    bool opened{ ImGui::TreeNodeEx(gameObject.get().get(), flag, "%s", gameObject->Name().c_str()) };

    // �I�������I�u�W�F�N�g���擾
    if (ImGui::IsItemClicked())
        s_selectObject = gameObject;

    if (opened)
    {
        for (const auto& child : gameObject->Transform()->Children())
        {
            DrawDebugParams(child->GameObject());
        }
        ImGui::TreePop();
    }
}

GameObjectPtr Glib::GameObjectManager::Instantiate()
{
    return gameObjects_.empty() ?
        Instantiate("GameObject") :
        Instantiate("GameObject " + std::to_string(gameObjects_.size()));
}

GameObjectPtr Glib::GameObjectManager::Instantiate(std::string_view name)
{
    const auto gameObject = std::make_shared<GameObject>(name);
    gameObjects_.push_back(gameObject);
    const WeakPtr<Internal::Interface::IGameObject> iGameObj{ gameObject };
    iGameObj->Initialize();
    return GameObjectPtr{ gameObject };
}

GameObjectPtr Glib::GameObjectManager::Find(std::string_view name)
{
    const auto& it = std::ranges::find_if(gameObjects_.begin(), gameObjects_.end(), [name](const std::shared_ptr<GameObject>& gameObject)
    {
        return gameObject->Name().compare(name);
    });

    return it != gameObjects_.end() ? GameObjectPtr{ *it } : GameObjectPtr{ nullptr };
}

std::list<GameObjectPtr> Glib::GameObjectManager::FindGameObjectsWithTag(std::string_view tag)
{
    std::list<GameObjectPtr> result;

    std::ranges::copy_if(gameObjects_.begin(), gameObjects_.end(), std::back_inserter(result), [tag](const std::shared_ptr<GameObject>& gameObject)
    {
        return gameObject->Tag().compare(tag);
    });

    return result;
}

GameObjectPtr Glib::GameObjectManager::FindGameObjectWithTag(std::string_view tag)
{
    const auto& it = std::ranges::find_if(gameObjects_.begin(), gameObjects_.end(), [tag](const std::shared_ptr<GameObject>& gameObject)
    {
        return gameObject->Tag().compare(tag);
    });

    return it != gameObjects_.end() ? GameObjectPtr{ *it } : GameObjectPtr{ nullptr };
}
