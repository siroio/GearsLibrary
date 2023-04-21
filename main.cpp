#include <Vector2.h>
#include <iostream>

int main()
{
    Vector2 a{ 10.0f, 30.0f };
    Vector2 b{ 100.0f, 60.0f };
    std::cout << a / b << std::endl;
}
