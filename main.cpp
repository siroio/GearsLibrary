#include <Singleton.h>
#include <iostream>

class DerivedSingleton : public Singleton<DerivedSingleton>
{
public:
    void Init()
    {
        std::cout << "Ptr ->" << _instance.get() << std::endl;
        std::cout << "Initializing DerivedSingleton..." << std::endl;
    }

    void Run()
    {
        std::cout << "Running DerivedSingleton..." << std::endl;
    }

    void Shutdown()
    {
        std::cout << "Shutting down DerivedSingleton..." << std::endl;
    }
};

int main()
{
    DerivedSingleton::Instance().Run();
    DerivedSingleton::Instance().Shutdown();

    DerivedSingleton::Destroy();

    DerivedSingleton::Instance().Run();
}
