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
     * @brief ゲームオブジェクト管理クラス
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
         * @brief GameObjectの作成
         * @return GameObjectPtr
         */
        static GameObjectPtr Instantiate();

        /**
         * @brief GameObjectの作成
         * @param name
         * @return GameObjectPtr
         */
        static GameObjectPtr Instantiate(std::string_view name);

        /**
         * @brief GameObjectの検索
         * @param name
         * @return GameObjectPtr
         */
        static GameObjectPtr Find(std::string_view name);

        /**
         * @brief GameObjectのTagで検索
         * @param tag
         * @return std::deque<GameObjectPtr>
         */
        static std::list<GameObjectPtr> FindGameObjectsWithTag(std::string_view tag);

        /**
         * @brief GameObjectのTagで検索
         * @param tag
         * @return GameObjectPtr
         */
        static GameObjectPtr FindGameObjectWithTag(std::string_view tag);

    private:
        static inline std::list<std::shared_ptr<GameObject>> gameObjects_;
    };
}
