#include "fassert.hpp"
#include <iostream>

int main() {
    int x = 0;

    FASSERT(x == 0)
        .hint("x={}", x);

    std::cout << "You can see me!" << std::endl;

    return 0;
}
