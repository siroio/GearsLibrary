// テスト用実行cpp

#include <iostream>
#include <Random.h>
#include <Game.h>
#include <WeakPtr.h>
#include <cassert>

using namespace std;
using namespace GLib::Utility;
class Base
{
public:
    virtual ~Base()
    {}
    virtual void foo()
    {
        std::cout << "Base::foo" << std::endl;
    }
};

class Derived : public Base
{
public:
    void bar()
    {
        std::cout << "Derived::bar" << std::endl;
    }
};

class MyClass : public Game
{
    virtual void Start() override
    {
        std::cout << "Testing WeakPtr" << std::endl;

        // Test shared_ptr constructor
        auto shared = std::make_shared<Derived>();
        WeakPtr<Base> weak_shared(shared);
        assert(weak_shared.get() == shared);

        // Test weak_ptr constructor
        std::weak_ptr<Base> weak_base(shared);
        WeakPtr<Base> weak_weak(weak_base);
        assert(weak_weak.get() == shared);

        // Test copy constructor
        WeakPtr<Base> weak_copy(weak_shared);
        assert(weak_copy.get() == shared);

        // Test upcast constructor
        WeakPtr<Derived> derived_weak(shared);
        WeakPtr<Base> base_weak(derived_weak);
        assert(base_weak.get() == shared);

        // Test downcast constructor
        WeakPtr<Base> base_downcast(shared);
        WeakPtr<Derived> derived_downcast(base_downcast);
        assert(derived_downcast.get() == shared);

        // Test operator->
        WeakPtr<Base> weak_op(weak_shared);
        weak_op->foo(); // Base::foo

        // Test operator*
        WeakPtr<Base> weak_deref(weak_shared);
        (*weak_deref).foo(); // Base::foo

        cout << weak_op.getId() << " == " << weak_deref.getId() << " = "
            << ((weak_op == weak_deref) ? "true" : "false") << endl;
        cout << "All tests passed!" << endl;
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
