#ifndef GEARS_GAME_OBJECT_MANAGER_H
#define GEARS_GAME_OBJECT_MANAGER_H

#include <Internal/ISystem.h>
#include <Singleton.h>
#include <FuncOrderDefinition.h>
#include <GameObjectPtr.h>
#include <list>
#include <string_view>

class GameObjectManager :
    public GLib::Internal::Interface::ISystem,
    public GLib::Utility::SingletonPtr<GameObjectManager>
{
public:
    void Update();
    void DebugDraw();
    void Finalize();
    void ResetActorList();

public:
    static GameObjectPtr CreateActor();
    static GameObjectPtr CreateActor(std::string_view name);
    static GameObjectPtr Find(std::string_view name);
    static std::list<GameObjectPtr> FindGameObjectsWithTag(std::string_view tag);
    static GameObjectPtr FindGameObjectWithTag(std::string_view tag);

private:
    static inline std::list<std::shared_ptr<GameObject>> gameObjects_;
};

#endif // !GEARS_GAME_OBJECT_MANAGER_H
