# Fancy Assert

A really fancy assertion facility for C++.

# Prerequisites

A header `prettyprint.hpp` must be accessable in the include-dir,
which can be found [here](https://github.com/TimeExceed/testa/tree/master/cpp).

# Usage

```c++
#include "fassert.hpp"

int main()
{
    int x = 0;

    // nothing happens
    FASSERT(x == 0)
        (x);

    // assert triggers.
    // and outputs `x` value, together with file name and line number
    FASSERT(x != 0)
        (x);

    // assert triggers.
    // and outputs `x` value, together with file name and line number
    // with an error message.
    FASSERT(x != 0)
        (x)
        .what("Something goes wrong!");

    return 0;
}
```

For some cases, programs must do some cleanups before aborting.

```c++
#include "fassert.hpp"

void cleanup()
{
    // do something
}

int main()
{
    fassert::Finalizers::singleton()->register_finalize(cleanup);

    // Assert triggers.
    // Before aborting, `cleanup()` will be invoked.
    FASSERT(false);

    return 0;
}
```
