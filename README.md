The Order Metalanguage for C Preprocessor Metaprogramming
=========================================================

> Order grew out of my interest in the design of programming languages and
> metaprogramming as well as desire to make something original, fascinating and
> beautiful.  My work on the
> [Boost Preprocessor](http://www.boost.org/doc/libs/1_55_0/libs/preprocessor/doc/index.html)-library
> had left me unsatisfied.  While it was clear to me that it could theoretically
> be used to implement *useful* non-trivial metaprograms, it would have been
> quite tedious work, full of nasty little details and limitations to trip you
> up.  I considered the prospects for further enhancements to the library, but
> it didn't seem possible to provide a programming model that I would have
> considered pleasant to think in.  It occured to me that instead of attempting
> to provide semimodular enhancements on top of the C preprocessor in a
> bottom-up fashion, it might be possible to lift the limitations by designing a
> complete monolithic interpreter for a high-level source language in a top-down
> fashion.  After some experiments at interpreting lambda-calculus and a couple
> of prototype interpreters for early versions of the Order language, I was left
> with a feeling that it would be too inefficient to be practical.  The most
> serious limitation seemed to be the inability of the early prototypes to
> generate arbitrary output efficiently.  I decided to forget about the
> interpreter and move on, but the work proved too interesting for me to forget
> completely.  Over a long period of time I found several small incremental
> improvements to the interpreter until it suddenly started to look like more
> than a theoretical curiosity.  Then I knew I just had to finish the work.
> Hopefully everyone will be able to find some beauty in the result.  At least I
> know I've had a lot of fun designing the language.

— *Vesa Karvonen*

---

## Overview

**Order** is an interpreted, purely functional metalanguage embedded entirely in the standard C/C++ preprocessor. It provides:

- **Higher-order functional programming** at preprocessor time (lambdas, closures, currying, recursion).
- **Arbitrary precision arithmetic** (integers of unbounded length computed during preprocessing).
- **Rich data structures**:
  - `tuple` (fixed-size records)
  - `seq` (token sequences)
  - `stream` (lazy/infinite sequences)
  - `map` (key-value lookup, insertion, deletion, and size)
- **Code generation**: generate repetitive C/C++ code, lookup tables, unrolled loops (e.g. Duff's device), and compile-time constants.

## Requirements & Dependencies

- C99 or C++11 compliant preprocessor (`gcc`, `clang`, `msvc`).
- [chaos-pp-x](https://github.com/bivex/chaos-pp-x) (Order utilizes `chaos/preprocessor` arbitrary precision and recursion backends).
- CMake 3.15+ (optional, for building tests & examples).

## Quick Start

### Basic Computation
```c
#include <order/interpreter.h>
#include <stdio.h>

int main(void) {
    // Computes 2 + 3 * 4 = 14 completely in the preprocessor
    int val = ORDER_PP(8to_lit(8add(2, 8mul(3, 4))));
    printf("Result: %d\n", val);
    return 0;
}
```

### Key-Value Map Operations (`order/lib/collections/map.h`)
```c
#include <order/interpreter.h>
#include <order/lib/collections/map.h>
#include <stdio.h>

int main(void) {
    // Create map and query properties using 8lets
    int size = ORDER_PP(8lets(
        (8M, 8map(8equal, (1, 10)(2, 20)(3, 30))),
        8to_lit(8map_size(8M))
    )); // 3

    int val = ORDER_PP(8lets(
        (8M, 8map(8equal, (1, 10)(2, 20)(3, 30))),
        8to_lit(8map_at(2, 8M))
    )); // 20

    int has_key = ORDER_PP(8lets(
        (8M, 8map(8equal, (1, 10)(2, 20)(3, 30))),
        8to_lit(8if(8map_exists(2, 8M), 1, 0))
    )); // 1

    printf("size: %d, val: %d, has_key: %d\n", size, val, has_key);
    return 0;
}
```

### Arbitrary Precision Fibonacci at Preprocessor Time
```c
#include <order/interpreter.h>
#include <stdio.h>

// Computes 500th Fibonacci number as a string literal at compile time
const char fib500[] = ORDER_PP(
    8to_lit(
        8fibonacci(8nat(5,0,0))
    )
);

int main(void) {
    printf("500th Fibonacci: %s\n", fib500);
    return 0;
}
```

## Building Tests & Examples

Order is a header-only library. A modern CMake and Makefile setup is included to build and run all tests and examples:

```bash
# Using Makefile
make
make test
make example

# Or using CMake directly
cmake -B build -G Ninja -DCHAOS_ROOT=/path/to/chaos-pp-x
cmake --build build
ctest --test-dir build --output-on-failure
```

## CMake Integration

Order exports the `order::order` target with its include directory configured.

```cmake
# When using FetchContent or add_subdirectory
add_subdirectory(order-pp-x)
target_link_libraries(my_target PRIVATE order::order)

# Or find_package after installation
find_package(order REQUIRED)
target_link_libraries(my_target PRIVATE order::order)
```

## License

Order is distributed under the Boost Software License, Version 1.0. See LICENSE for details.

