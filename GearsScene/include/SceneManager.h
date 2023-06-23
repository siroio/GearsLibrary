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
#include <StringUtility.h>

namespace GLib::Scene
{
    class Scene;

    /**
     * @brief �V�[���Ǘ��N���X
     */
    class SceneManager :
        public GLib::Internal::Interface::ISystem,
        public GLib::SingletonPtr<SceneManager>,
        public GLib::Internal::Function::UpdateOrderSet<GLib::Constant::Order::Update::SCENE>
    {
        SceneManager() = default;
        friend GLib::WeakPtr<SceneManager> GLib::SingletonPtr<SceneManager>::Instance();

    public:

        /**
         * @brief �V�[���̃��[�h
         */
        void Update();

        /**
         * @brief �V�[���̓o�^
         */
        template<class SceneType>
        static void Register();

        /**
         * @brief �V�[���̓ǂݍ���
         *
         * @param �V�[����
         */
        static void LoadScene(const std::string& name);

    private:
        static inline std::unordered_map<std::string, std::shared_ptr<Scene>> scenes_;
        static inline GLib::WeakPtr<Scene> current_;
        static inline GLib::WeakPtr<Scene> next_;
    };

    template<class SceneType>
    inline void SceneManager::Register()
    {
        scenes_.emplace(Nameof<SceneType>(), std::make_shared<SceneType>());
    }
}

#endif // !GEARS_SCENE_MANAGER_H
