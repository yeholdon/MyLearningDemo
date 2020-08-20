#include <iostream>

struct foo
{
    //foo() = default;
    int a;
};

struct bar
{
    bar();
    int b;
};

bar::bar() = default;

int main()
{
    foo a{};
    bar b{};

    std::cout << a.a << '\t' << b.b;
}