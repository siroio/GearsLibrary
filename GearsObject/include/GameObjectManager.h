#ifndef GEARS_GAME_OBJECT_MANAGER_H
#define GEARS_GAME_OBJECT_MANAGER_H

#include <Internal/ISystem.h>
#include <Singleton.h>
#include <FuncOrderDefinition.h>
#include <GameObjectPtr.h>
#include <deque>
#include <string_view>

/**
 * @brief ゲームオブジェクト管理クラス
 */
class GameObjectManager final :
    public GLib::Internal::Interface::ISystem,
    public GLib::SingletonPtr<GameObjectManager>
{
private:
    friend GLib::WeakPtr<GameObjectManager> GLib::SingletonPtr<GameObjectManager>::Instance();
    GameObjectManager() = default;

public:
    void Update();
    void DebugDraw();
    void Finalize();
    void ResetActorList();

public:

    /**
     * @brief GameObjectの作成
     * @return GameObjectPtr
     */
    static GameObjectPtr CreateActor();

    /**
     * @brief GameObjectの作成
     * @param name
     * @return GameObjectPtr
     */
    static GameObjectPtr CreateActor(std::string_view name);

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
    static std::deque<GameObjectPtr> FindGameObjectsWithTag(std::string_view tag);

    /**
     * @brief GameObjectのTagで検索
     * @param tag
     * @return GameObjectPtr
     */
    static GameObjectPtr FindGameObjectWithTag(std::string_view tag);

private:
    static inline std::deque<std::shared_ptr<GameObject>> gameObjects_;
};

#endif // !GEARS_GAME_OBJECT_MANAGER_H
