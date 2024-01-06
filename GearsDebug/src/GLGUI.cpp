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
    if (ImGui::BeginCombo(label.data(), curerntItem.c_str()))
    {
        for (const auto& item : items)
        {
            bool is_selected = (curerntItem == item);
            if (ImGui::Selectable(item.c_str(), is_selected))
            {
                curerntItem = item;
                return true;
            }

            if (is_selected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    return false;
}

bool Glib::GLGUI::TreeNode(std::string_view label, bool leaf)
{
    return ImGui::TreeNodeEx(label.data(), leaf ? GL_TREE_LEAF : GL_TREE_NODE);
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

bool Glib::GLGUI::CoorPicker3(std::string_view label, Color* color, bool sameLine)
{
    if (sameLine) ImGui::SameLine();
    return ImGui::ColorPicker3(label.data(), color->Raw(), GL_COLOR_FLAG);
}

bool Glib::GLGUI::CoorPicker4(std::string_view label, Color* color, bool sameLine)
{
    if (sameLine) ImGui::SameLine();
    return ImGui::ColorPicker4(label.data(), color->Raw(), GL_COLOR_FLAG);
}
