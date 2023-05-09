// テスト用実行cpp

#include <iostream>
#include <Random.h>
#include <Game.h>
using namespace std;

class MyClass : public Game
{
    virtual void Start() override
    {
        for (size_t i = 0; i < 100; i++)
        {
            printf("%d \n", Random::Instance().Range(0, 10));
        }
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
