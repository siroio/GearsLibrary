#include <GameObjectManager.h>
#include <Internal/IGameObject.h>
#include <Internal/ImGuiInc.h>
#include <GLGUI.h>
#include <GameObject.h>
#include <WeakPtr.h>
#include <algorithm>
#include <iterator>
#include <StringUtility.h>

namespace
{
    bool s_enableHierarchy{ true };
    bool s_enableProperties{ true };
    GameObjectPtr s_selectObject{ nullptr };
}

void Glib::GameObjectManager::Update()
{
    // Deadフラグの立ったコンポーネントを削除
    for (const auto& gameObject : gameObjects_)
    {
        gameObject->RemoveDeadComponents();
    }

    // Deadフラグの立ったオブジェクトを削除
    gameObjects_.remove_if([](const std::shared_ptr<GameObject>& gameObject)
    {
        return gameObject->IsDead();
    });
}

void Glib::GameObjectManager::DebugDraw()
{
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Window"))
    {
        GLGUI::CheckBox("Hierarchy", &s_enableHierarchy);
        GLGUI::CheckBox("Properties", &s_enableProperties);
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    if (s_enableHierarchy)
    {
        ImGui::Begin("Hierarchy", &s_enableHierarchy);
        ImGui::Separator();
        for (const auto& gameObject : gameObjects_)
        {
            // パラメーター表示
            if (!gameObject->IsRoot()) continue;
            DrawDebugParameter(gameObject);
        }
        ImGui::End();
    }

    if (s_enableProperties)
    {
        ImGui::Begin("Properties", &s_enableProperties);
        ImGui::Separator();
        if (!s_selectObject.expired())
        {
            ImGuiInputTextFlags flags =
                ImGuiInputTextFlags_AutoSelectAll |
                ImGuiInputTextFlags_EnterReturnsTrue;

            // アクティブフラグの表示
            bool active = s_selectObject->Active();
            if (ImGui::Checkbox("##Active", &active))
            {
                s_selectObject->Active(active);
            };
            ImGui::SameLine();

            // 名前の表示
            auto name = s_selectObject->Name();
            if (ImGui::InputText("##Name", name.data(), name.size(), flags))
            {
                s_selectObject->Name(name);
            }

            // レイヤーの表示
            std::string tag = "Tag: " + (s_selectObject->Tag().empty() ? "None" : s_selectObject->Tag());
            ImGui::Text(tag.c_str());
            ImGui::SameLine();

            // レイヤーの表示
            std::string layer = "Layer: " + std::to_string(s_selectObject->Layer());
            ImGui::Text(layer.c_str());
            ImGui::Separator();

            // コンポーネントの表示
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

void Glib::GameObjectManager::DrawDebugParameter(GameObjectPtr gameObject)
{
    ImGuiTreeNodeFlags flag =
        ImGuiTreeNodeFlags_OpenOnArrow |
        ImGuiTreeNodeFlags_OpenOnDoubleClick;

    // 葉ノード
    if (gameObject->Transform()->Children().empty())
        flag |= ImGuiTreeNodeFlags_Leaf;

    // 選択されているか
    if (gameObject == s_selectObject)
        flag |= ImGuiTreeNodeFlags_Selected;

    auto name = gameObject->Name();
    bool opened{ ImGui::TreeNodeEx(gameObject.get().get(), flag, "%s", name.data()) };

    // 選択したオブジェクトを取得
    if (ImGui::IsItemClicked())
        s_selectObject = gameObject;

    if (opened)
    {
        // 子供を描画
        for (const auto& child : gameObject->Transform()->Children())
        {
            DrawDebugParameter(child->GameObject());
        }
        ImGui::TreePop();
    }
}

GameObjectPtr Glib::GameObjectManager::Instantiate()
{
    return Instantiate("GameObject");
}

GameObjectPtr Glib::GameObjectManager::Instantiate(std::string_view name)
{
    auto gameObject = std::make_shared<GameObject>(name);
    gameObjects_.push_back(gameObject);
    WeakPtr<Internal::Interface::IGameObject> iGameObject{ gameObject };
    iGameObject->Initialize();
    return GameObjectPtr{ gameObject };
}

GameObjectPtr Glib::GameObjectManager::Find(std::string_view name)
{
    const auto& it = std::ranges::find_if(gameObjects_, [name](const std::shared_ptr<GameObject>& gameObject)
    {
        return gameObject->Name() == name;
    });

    return it != gameObjects_.end() ? GameObjectPtr{ *it } : GameObjectPtr{ nullptr };
}

std::list<GameObjectPtr> Glib::GameObjectManager::FindGameObjectsWithTag(std::string_view tag)
{
    std::list<GameObjectPtr> result;
    std::ranges::copy_if(gameObjects_, std::back_inserter(result), [tag](const std::shared_ptr<GameObject>& gameObject)
    {
        return gameObject->Tag() == tag;
    });

    return result;
}

GameObjectPtr Glib::GameObjectManager::FindGameObjectWithTag(std::string_view tag)
{
    const auto& it = std::ranges::find_if(gameObjects_.begin(), gameObjects_.end(), [tag](const std::shared_ptr<GameObject>& gameObject)
    {
        return gameObject->Tag() == tag;
    });

    return it != gameObjects_.end() ? GameObjectPtr{ *it } : GameObjectPtr{ nullptr };
}
