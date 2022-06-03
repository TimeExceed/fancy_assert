#include "fassert.hpp"
#include <cstdio>

void cleanup() noexcept
{
    fprintf(stderr, "cleanup\n");
}

int main() {
    int x = 0;

    fassert::Finalizers::singleton()->register_finalizer(cleanup);

    // Assert triggers.
    // Before aborting, `cleanup()` will be invoked.
    FASSERT(x != 0)
        (x);

    return 0;
}
