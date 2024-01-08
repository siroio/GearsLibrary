#include <Components/DirectionalLight.h>
#include <Internal/RenderingManager.h>
#include <GameObject.h>
#include <GLGUI.h>
#include <Color.h>
#include <imgui.h>

using namespace Glib::Internal::Graphics;

void Glib::DirectionalLight::Start()
{
    transform_ = GameObject()->Transform();
}

void Glib::DirectionalLight::LateUpdate()
{
    RenderingManager::LightDirection(transform_->Forward());
}

void Glib::DirectionalLight::Ambient(const Color& ambient)
{
    RenderingManager::LightAmbient(ambient);
}

void Glib::DirectionalLight::Diffuse(const Color& diffuse)
{
    RenderingManager::LightDiffuse(diffuse);
}

void Glib::DirectionalLight::Specular(const Color& specular)
{
    RenderingManager::LightSpecular(specular);
}

void Glib::DirectionalLight::OnGUI()
{
    Component::OnGUI();
    if (GLGUI::TreeNode("Ambient"))
    {
        Color ambient = RenderingManager::LightAmbient();
        if (GLGUI::ColorInput3("##Ambient", &ambient))
            Ambient(ambient);
        GLGUI::TreePop();
    }

    if (GLGUI::TreeNode("Diffuse"))
    {
        Color diffuse = RenderingManager::LightDiffuse();
        if (GLGUI::ColorInput3("##Diffuse", &diffuse))
            Diffuse(diffuse);
        GLGUI::TreePop();
    }

    if (GLGUI::TreeNode("Specular"))
    {
        Color specular = RenderingManager::LightSpecular();
        if (GLGUI::ColorInput3("##Specular", &specular))
            Specular(specular);
        GLGUI::TreePop();
    }
}
