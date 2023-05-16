// テスト用実行cpp

#include <iostream>
#include <Random.h>
#include <Game.h>
#include <WeakPtr.h>
#include <cassert>

using namespace std;

class MyClass : public Game
{
    virtual void Start() override
    {
        cout << "START" << endl;
    }

    virtual void End() override
    {
        cout << "END" << endl;
    }
};


auto main() -> int
{
    MyClass{}.Run();
}
