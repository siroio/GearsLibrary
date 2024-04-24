#include <GLGUI.h>
#include <Internal/ImGuiInc.h>
#include <Vector2.h>
#include <Vector3.h>
#include <Vector4.h>
#include <Color.h>

void Glib::GLGUI::Space()
{
    ImGui::Spacing();
}

void Glib::GLGUI::Separator()
{
    ImGui::Separator();
}

void Glib::GLGUI::Text(std::string_view fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    ImGui::TextV(fmt.data(), args);
    va_end(args);
}

bool Glib::GLGUI::TabHeader(std::string_view label, bool open)
{
    return ImGui::CollapsingHeader(label.data(), open ? ImGuiTreeNodeFlags_DefaultOpen : ImGuiTreeNodeFlags_None);
}

bool Glib::GLGUI::Button(std::string_view label, bool sameLine)
{
    if (sameLine) ImGui::SameLine();
    return ImGui::Button(label.data());
}

bool Glib::GLGUI::CheckBox(std::string_view label, bool* checked, bool sameLine)
{
    if (sameLine) ImGui::SameLine();
    return ImGui::Checkbox(label.data(), checked);
}

bool Glib::GLGUI::ComboBox(std::string_view label, std::string& curerntItem, const std::vector<std::string>& items)
{
    bool clicked{ false };
    if (ImGui::BeginCombo(label.data(), curerntItem.c_str()))
    {
        for (const auto& item : items)
        {
            bool is_selected = (curerntItem == item);
            if (ImGui::Selectable(item.c_str(), is_selected))
            {
                curerntItem = item;
                clicked = true;
            }

            if (is_selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    return clicked;
}

bool Glib::GLGUI::TreeNode(std::string_view label, bool leaf, bool openNode)
{
    ImGuiTreeNodeFlags flag = (leaf ? GL_TREE_LEAF : GL_TREE_NODE);
    if (openNode) flag |= ImGuiTreeNodeFlags_DefaultOpen;
    return ImGui::TreeNodeEx(label.data(), flag);
}

void Glib::GLGUI::TreePop()
{
    ImGui::TreePop();
}

bool Glib::GLGUI::InputInt(std::string_view label, int* value, bool sameLine)
{
    if (sameLine) ImGui::SameLine();
    return ImGui::InputInt(label.data(), value, 1, 0, GL_ENTER_RETURN);
}

bool Glib::GLGUI::InputFloat(std::string_view label, float* value, bool sameLine)
{
    if (sameLine) ImGui::SameLine();
    return ImGui::InputFloat(label.data(), value, 0.0f, 0.0f, "%.3f", GL_ENTER_RETURN);
}

bool Glib::GLGUI::InputVector2(std::string_view label, Vector2* value, bool sameLine)
{
    if (sameLine) ImGui::SameLine();
    return ImGui::InputFloat2(label.data(), value->xy.data(), "%.3f", GL_ENTER_RETURN);
}

bool Glib::GLGUI::InputVector3(std::string_view label, Vector3* value, bool sameLine)
{
    if (sameLine) ImGui::SameLine();
    return ImGui::InputFloat3(label.data(), value->xyz.data(), "%.3f", GL_ENTER_RETURN);
}

bool Glib::GLGUI::InputVector4(std::string_view label, Vector4* value, bool sameLine)
{
    if (sameLine) ImGui::SameLine();
    return ImGui::InputFloat4(label.data(), value->xyzw.data(), "%.3f", GL_ENTER_RETURN);
}

bool Glib::GLGUI::DragInt(std::string_view label, int* value, float speed, int min, int max, bool sameLine)
{
    if (sameLine) ImGui::SameLine();
    return ImGui::DragInt(label.data(), value, speed, min, max, "%d", GL_SLIDER_CLAMP);
}

bool Glib::GLGUI::DragFloat(std::string_view label, float* value, float speed, float min, float max, bool sameLine)
{
    if (sameLine) ImGui::SameLine();
    return ImGui::DragFloat(label.data(), value, speed, min, max, "%.3f", GL_SLIDER_CLAMP);
}

bool Glib::GLGUI::DragVector2(std::string_view label, Vector2* value, float speed, float min, float max, bool sameLine)
{
    if (sameLine) ImGui::SameLine();
    return ImGui::DragFloat2(label.data(), value->xy.data(), speed, min, max, "%.3f", GL_SLIDER_CLAMP);
}

bool Glib::GLGUI::DragVector3(std::string_view label, Vector3* value, float speed, float min, float max, bool sameLine)
{
    if (sameLine) ImGui::SameLine();
    return ImGui::DragFloat3(label.data(), value->xyz.data(), speed, min, max, "%.3f", GL_SLIDER_CLAMP);
}

bool Glib::GLGUI::DragVector4(std::string_view label, Vector4* value, float speed, float min, float max, bool sameLine)
{
    if (sameLine) ImGui::SameLine();
    return ImGui::DragFloat4(label.data(), value->xyzw.data(), speed, min, max, "%.3f", GL_SLIDER_CLAMP);
}

bool Glib::GLGUI::SliderInt(std::string_view label, int* value, int min, int max, bool sameLine)
{
    if (sameLine) ImGui::SameLine();
    return ImGui::SliderInt(label.data(), value, min, max, "%d", GL_SLIDER_CLAMP);
}

bool Glib::GLGUI::SliderFloat(std::string_view label, float* value, float min, float max, bool sameLine)
{
    if (sameLine) ImGui::SameLine();
    return ImGui::SliderFloat(label.data(), value, min, max, "%.3f", GL_SLIDER_CLAMP);
}

bool Glib::GLGUI::ColorPicker3(std::string_view label, Color* color, bool sameLine)
{
    if (sameLine) ImGui::SameLine();
    return ImGui::ColorPicker3(label.data(), color->Raw(), GL_COLOR_PICK_FLAG);
}

bool Glib::GLGUI::ColorPicker4(std::string_view label, Color* color, bool sameLine)
{
    if (sameLine) ImGui::SameLine();
    return ImGui::ColorPicker4(label.data(), color->Raw(), GL_COLOR_PICK_FLAG);
}

bool Glib::GLGUI::ColorInput3(std::string_view label, Color* color, bool preview, bool sameLine)
{
    if (sameLine) ImGui::SameLine();
    return ImGui::ColorEdit3(label.data(), color->Raw(), GL_COLOR_INPUT_FLAG);
}

bool Glib::GLGUI::ColorInput4(std::string_view label, Color* color, bool preview, bool sameLine)
{
    if (sameLine) ImGui::SameLine();
    return ImGui::ColorEdit4(label.data(), color->Raw(), GL_COLOR_INPUT_FLAG);
}

bool Glib::GLGUI::ColorBar3(std::string_view label, Color* color)
{
    return ImGui::ColorEdit3(label.data(), color->Raw(), ImGuiColorEditFlags_NoInputs);
}

bool Glib::GLGUI::ColorBar4(std::string_view label, Color* color)
{
    return ImGui::ColorEdit4(label.data(), color->Raw(), ImGuiColorEditFlags_NoInputs);
}
