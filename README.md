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

In some cases, programs have to output the triggered message to a customized
sink and then do some cleanups before aborting.

```c++
#include "fassert.hpp"
#include <spdlog/spdlog.h>

void sink_to_spdlog(std::string_view msg)
{
    spdog::error("Assertion triggered: {}", msg);
    spdlog::shutdown();
}

int main()
{
    fassert::Finalizers::singleton()->clear(); // clear the default finalizer
    fassert::Finalizers::singleton()->register_finalizer(sink_to_spdlog);

    // Assert triggers.
    // Before aborting, `sink_to_spdlog()` will be invoked.
    FASSERT(false);

    return 0;
}
```
