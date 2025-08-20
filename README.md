# Fancy Assert

A really fancy assertion facility for C++.

# Prerequisites

Either C++20 (for `std::vformat`, `std::make_format_args` and `std::format_to`) or [fmtlib](https://fmt.dev/) is required.
* To use C++20, `ENABLE_STD_FORMAT` must be defined.
* To use fmtlib, `ENABLE_FMTLIB` must be defined.

# Usage

```c++
#include "fassert.hpp"

int main()
{
    int x = 0;

    // nothing happens
    FASSERT(x == 0)
        .hint("x={}", x);

    // the assert triggers, and outputs to stderr
    //
    // ```
    // Assertion fails:
    //   LOCATION: /media/data/personal/fancy_assert/examples/trigger.cpp:7
    //   FUNCTION: main
    //   Condition: x != 0
    //   x=0
    // ```
    FASSERT(x != 0)
        .hint("x={}", x);

    // the assert triggers, and outputs to stderr with a customized explanation
    //
    // ```
    // Assertion fails: Something goes wrong!
    //   LOCATION: /media/data/personal/fancy_assert/examples/what.cpp:7
    //   FUNCTION: main
    //   Condition: x != 0
    //   x=0
    // ```
    FASSERT(x != 0)
        .hint("x={}", x);
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
