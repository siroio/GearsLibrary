#pragma once
#include <Internal/ISystem.h>
#include <Internal/FuncOrderConstant.h>
#include <FuncOrderDefinition.h>
#include <Singleton.h>
#include <WeakPtr.h>
#include <StringUtility.h>
#include <memory>
#include <unordered_map>
#include <list>

namespace Glib
{
    class Scene;

    /**
     * @brief �V�[���Ǘ��N���X
     */
    class SceneManager :
        public Internal::Interface::ISystem,
        public SingletonPtr<SceneManager>,
        public Internal::Function::UpdateOrderSet<Internal::Order::Update::SCENE>
    {
        SceneManager() = default;
        friend WeakPtr<SceneManager> SingletonPtr<SceneManager>::Instance();

    public:

        /**
         * @brief �V�[���̃��[�h
         */
        void Update();

        /**
         * @brief �V�[���̓o�^
         */
        template<class Scene>
        static void Register();

        /**
         * @brief �V�[���̓ǂݍ���
         *
         * @param �V�[����
         */
        static void LoadScene(const std::string& name);

        /**
         * @brief �V�[�����疼�O���擾
         */
        template<class Scene>
        static std::string SceneName();

        /**
         * @brief �o�^���ꂽ�V�[���ꗗ 
         */
        static std::list<std::string> Scenes();

    private:
        static inline std::unordered_map<std::string, std::shared_ptr<Scene>> scenes_;
        static inline Glib::WeakPtr<Scene> current_;
        static inline Glib::WeakPtr<Scene> next_;
    };

    template<class Scene>
    inline void SceneManager::Register()
    {
        scenes_.emplace(nameof<Scene>(), std::make_shared<Scene>());
    }

    template<class Scene>
    inline std::string SceneManager::SceneName()
    {
        auto name = nameof<Scene>();
        if (!scenes_.contains(name)) return "NotFound";
        return name;
    }
}
