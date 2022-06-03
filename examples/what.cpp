#include "fassert.hpp"

int main() {
    int x = 0;

    FASSERT(x != 0)
        (x)
        .what("Something goes wrong!");

    return 0;
}
