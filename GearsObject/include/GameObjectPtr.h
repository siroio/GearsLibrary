#ifndef GEARS_GAME_OBJECT_PTR_H
#define GEARS_GAME_OBJECT_PTR_H

#include <WeakPtr.h>

class GameObject;
using GameObjectPtr = Glib::WeakPtr<GameObject>;

#endif // !GEARS_GAME_OBJECT_PTR_H
