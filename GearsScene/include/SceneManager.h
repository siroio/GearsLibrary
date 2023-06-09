#ifndef GEARS_SCENE_MANAGER_H
#define GEARS_SCENE_MANAGER_H

#include <Internal/ISystem.h>
#include <Internal/FuncOrderConstant.h>
#include <FuncOrderDefinition.h>
#include <Singleton.h>
#include <WeakPtr.h>

#include <unordered_map>
#include <memory>
#include <string>

namespace GLib::Scene
{
    class Scene;

    class SceneManager :
        public GLib::Internal::Interface::ISystem,
        public GLib::Utility::SingletonPtr<SceneManager>,
        public GLib::Internal::Function::UpdateOrderSet<GLib::Constant::Order::Update::SCENE>
    {
        SceneManager() = default;
        friend GLib::Utility::WeakPtr<SceneManager> GLib::Utility::SingletonPtr<SceneManager>::Instance();

    public:
        void Update();

        /**
         * @brief �V�[���̓o�^
         */
        template<class SceneType>
        static void Register(SceneType Scene);

        /**
         * @brief �V�[���̓ǂݍ���
         *
         * @param �V�[����
         */
        static void LoadScene(const std::string& name);

    private:
        static std::unordered_map<std::string, std::shared_ptr<Scene>> scenes_;
        static GLib::Utility::WeakPtr<Scene> current_;
        static GLib::Utility::WeakPtr<Scene> next_;
    };

    template<class SceneType>
    inline void SceneManager::Register(SceneType Scene)
    {
        std::string name = typeid(SceneType).name();
        std::size_t start = name.find(' ');
        if (start != std::string::npos) name = name.substr(start + 1);

        std::size_t end = name.rfind("::");
        if (end != std::string::npos) name = name.substr(end + 2);

        scenes_.emplace(name, std::make_shared<SceneType>());
    }
}

#endif // !GEARS_SCENE_MANAGER_H
