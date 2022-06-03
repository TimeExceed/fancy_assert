#include "fassert.hpp"

int main() {
    int x = 0;

    FASSERT(x == 0)
        (x);

    return 0;
}
