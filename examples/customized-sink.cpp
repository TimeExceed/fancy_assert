#include "fassert.hpp"
#include <cstdio>

void customized_sink(std::string_view msg) noexcept
{
    fprintf(stderr, "triggered msg to customized sink: %s\n", msg.data());
}

int main() {
    fassert::Finalizers::singleton()->clear(); // clear the default finalizer
    fassert::Finalizers::singleton()->register_finalizer(customized_sink);

    int x = 0;
    // Assert triggers.
    // Before aborting, `cleanup()` will be invoked.
    FASSERT(x != 0)
        ("x={}", x);

    return 0;
}
