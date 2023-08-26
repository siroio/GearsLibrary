#pragma once
#include <Internal/ISystem.h>
#include <Singleton.h>
#include <FuncOrderDefinition.h>
#include <GameObjectPtr.h>
#include <string_view>
#include <list>

namespace Glib
{
    /**
     * @brief �Q�[���I�u�W�F�N�g�Ǘ��N���X
     */
    class GameObjectManager final :
        public Glib::Internal::Interface::ISystem,
        public Glib::SingletonPtr<GameObjectManager>
    {
    private:
        friend Glib::WeakPtr<GameObjectManager> Glib::SingletonPtr<GameObjectManager>::Instance();
        GameObjectManager() = default;

    public:
        void Update();
        void DebugDraw();
        void Finalize();
        void ResetGameObjects();

    public:

        /**
         * @brief GameObject�̍쐬
         * @return GameObjectPtr
         */
        static GameObjectPtr Instantiate();

        /**
         * @brief GameObject�̍쐬
         * @param name
         * @return GameObjectPtr
         */
        static GameObjectPtr Instantiate(std::string_view name);

        /**
         * @brief GameObject�̌���
         * @param name
         * @return GameObjectPtr
         */
        static GameObjectPtr Find(std::string_view name);

        /**
         * @brief GameObject��Tag�Ō���
         * @param tag
         * @return std::deque<GameObjectPtr>
         */
        static std::list<GameObjectPtr> FindGameObjectsWithTag(std::string_view tag);

        /**
         * @brief GameObject��Tag�Ō���
         * @param tag
         * @return GameObjectPtr
         */
        static GameObjectPtr FindGameObjectWithTag(std::string_view tag);

    private:
        static inline std::list<std::shared_ptr<GameObject>> gameObjects_;
    };
}
