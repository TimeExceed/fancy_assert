#include "fassert.hpp"
#include <iostream>

void customized_sink(std::string_view msg) noexcept
{
    std::cerr << "triggered msg to customized sink: " << msg << std::endl;
}

int main() {
    fassert::Finalizers::singleton()->clear(); // clear the default finalizer
    fassert::Finalizers::singleton()->register_finalizer(customized_sink);

    int x = 0;
    FASSERT(x != 0)
        .hint("x={}", x);

    std::cout << "You can't see me!" << std::endl;

    return 0;
}
