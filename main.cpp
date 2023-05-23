// テスト用実行cpp

#include <iostream>
#include <Random.h>
#include <Game.h>
#include <WeakPtr.h>
#include <cassert>
#include <GameObjectManager.h>
#include <FuncOrderDefinition.h>
#include <Internal/IFunc.h>
#include <FuncDefinition.h>
using namespace std;
using namespace GLib;

class MyClass : public Game
{
    virtual void Start() override
    {}

    virtual void End() override
    {
        cout << "END" << endl;
    }
};

auto main() -> int
{
    MyClass{}.Run();
}
