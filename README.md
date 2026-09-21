<div align="center">

# Order Metalanguage (`order-pp-x`)

### An interpreted, purely functional metalanguage embedded entirely in the standard C/C++ preprocessor

[![License: BSL-1.0](https://img.shields.io/badge/License-Boost_1.0-blue.svg)](https://opensource.org/licenses/BSL-1.0)
[![Standard: C99](https://img.shields.io/badge/Standard-C99-brightgreen.svg)](#requirements--dependencies)
[![Standard: C++11](https://img.shields.io/badge/Standard-C%2B%2B11-brightgreen.svg)](#requirements--dependencies)
[![Build & Tests](https://img.shields.io/badge/CTest-100%25_Passing-success.svg)](#building-tests--examples)
[![Dependency: chaos-pp-x](https://img.shields.io/badge/Dependency-chaos--pp--x-orange.svg)](https://github.com/bivex/chaos-pp-x)
[![CMake: 3.15+](https://img.shields.io/badge/CMake-3.15%2B-informational.svg)](#cmake-integration)

</div>

---

> [!NOTE]
> *"Order grew out of my interest in the design of programming languages and metaprogramming as well as desire to make something original, fascinating and beautiful... It occurred to me that instead of attempting to provide semimodular enhancements on top of the C preprocessor in a bottom-up fashion, it might be possible to lift the limitations by designing a complete monolithic interpreter for a high-level source language in a top-down fashion... Hopefully everyone will be able to find some beauty in the result."*
> — **Vesa Karvonen**, original author

---

## Table of Contents

- [Overview](#overview)
- [Key Features & Architecture](#key-features--architecture)
  - [1. Higher-Order Functional Programming](#1-higher-order-functional-programming)
  - [2. Arbitrary Precision BigInt Arithmetic](#2-arbitrary-precision-bigint-arithmetic)
  - [3. Rich First-Class Collections](#3-rich-first-class-collections)
  - [4. Monolithic Top-Down Virtual Machine](#4-monolithic-top-down-virtual-machine)
- [Quick Start](#quick-start)
  - [Compile-Time Arithmetic](#compile-time-arithmetic)
  - [Key-Value Map Dictionary](#key-value-map-dictionary)
  - [Arbitrary Precision 500th Fibonacci Number](#arbitrary-precision-500th-fibonacci-number)
  - [Lazy & Infinite Streams](#lazy--infinite-streams)
- [Requirements & Dependencies](#requirements--dependencies)
- [Building Tests & Examples](#building-tests--examples)
- [CMake Integration](#cmake-integration)
- [Quality Assurance & Bug Fixes](#quality-assurance--bug-fixes)
- [License](#license)

---

## Overview

**Order** transforms the standard C and C++ preprocessor into an execution environment for an expressive, interpreted, purely functional language. Rather than relying on clumsy nested macro tricks, Order provides a high-level language with:

- **Lexical Scoping & Variables** (`8let`, `8lets`)
- **First-Class Anonymous Functions (Lambdas)** (`8fn`, closures, currying)
- **First-Class Continuations** (`8call_cc`)
- **Arbitrary-Precision Integers** (unbounded arithmetic during preprocessing)
- **Abstract Data Types**: Sequences (`seq`), Tuples (`tuple`), Lazy Streams (`stream`), and Key-Value Maps (`map`)
- **Automated Code Generation**: Unrolled loops, Duff's Device, lookup tables, and AST parsers

---

## Key Features & Architecture

### 1. Higher-Order Functional Programming
Order treats functions as first-class citizens. You can construct anonymous lambdas (`8fn`), pass functions into higher-order algorithms (`8seq_map`, `8seq_filter`, `8seq_fold`), curry arguments, and capture local scope:

```c
// Preprocessor lambda calculating (x * 2) + y
8fn(8X, 8Y, 8add(8mul(8X, 2), 8Y))
```

### 2. Arbitrary Precision BigInt Arithmetic
Unlike standard C macros bounded by compiler word size, Order features arbitrary precision arithmetic powered by `chaos-pp-x`:
- Unbounded natural numbers (`8nat`) and integers.
- Operations: `8add`, `8sub`, `8mul`, `8div`, `8mod`, `8pow`, `8gcd`, `8lcm`.
- All evaluated strictly during preprocessing, leaving **zero runtime footprint**.

### 3. Rich First-Class Collections
| Data Structure | Description | Key Operations |
|---|---|---|
| **`seq`** | Sequential token lists `(a)(b)(c)` | `8seq_map`, `8seq_filter`, `8seq_fold`, `8seq_reverse`, `8seq_zip` |
| **`tuple`** | Heterogeneous fixed-size records `(a, b, c)` | `8tuple_at`, `8tuple_size`, `8pair` |
| **`stream`** | Lazy/infinite evaluation streams | `8stream_cons`, `8stream_head`, `8stream_tail`, `8stream_merge`, `8stream_take` |
| **`map`** | Associative key-value dictionary | `8map`, `8map_at`, `8map_exists`, `8map_insert`, `8map_erase`, `8map_union` |

### 4. Monolithic Top-Down Virtual Machine
Order replaces ad-hoc macro expansion with an interpretive continuation-machine loop. An Order program inside `ORDER_PP(...)` is parsed into VM instructions and executed step-by-step through trampoline expansions until reaching final normal form.

---

## Quick Start

### Compile-Time Arithmetic
```c
#include <order/interpreter.h>
#include <stdio.h>

int main(void) {
    // Computes 2 + (3 * 4) = 14 during preprocessing
    int val = ORDER_PP(8to_lit(8add(2, 8mul(3, 4))));
    printf("Result: %d\n", val); // 14
    return 0;
}
```

### Key-Value Map Dictionary
```c
#include <order/interpreter.h>
#include <order/map.h> // or <order/lib/collections/map.h>
#include <stdio.h>

int main(void) {
    // Construct map and query elements using serial binding (8lets)
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

### Arbitrary Precision 500th Fibonacci Number
```c
#include <order/interpreter.h>
#include <stdio.h>

// Evaluated completely at compile time:
const char fib500[] = ORDER_PP(
    8to_lit(
        8fibonacci(8nat(5, 0, 0))
    )
);

int main(void) {
    printf("500th Fibonacci:\n%s\n", fib500);
    // Output: 139423224561697880139724382870407283950070256587697307264108962948325571622863290691557658876222521294125
    return 0;
}
```

### Lazy & Infinite Streams
```c
#include <order/interpreter.h>
#include <order/lib/stream.h>
#include <stdio.h>

int main(void) {
    // Infinite stream of natural numbers: (0, 1, 2, ...)
    // Take the 5th element:
    int elem = ORDER_PP(8to_lit(
        8stream_head(
            8stream_drop(4, 8stream_of_naturals)
        )
    ));
    printf("5th natural number: %d\n", elem); // 4
    return 0;
}
```

---

## Requirements & Dependencies

- **C/C++ Preprocessor**: Any standard-compliant C99 or C++11 preprocessor (`clang`, `gcc`, `msvc`).
- **[chaos-pp-x](https://github.com/bivex/chaos-pp-x)**: Order leverages `chaos/preprocessor` recursion engines and arbitrary precision arithmetic backend.
- **CMake**: 3.15+ (optional, for running tests and examples).

---

## Building Tests & Examples

Order is a header-only library. A build system is provided to compile and verify all test suites and examples:

```bash
# Using Makefile
make
make test      # Runs CTest suite (C99 & C++11)
make example   # Builds and executes all examples
make clean

# Or using CMake directly
cmake -B build -G Ninja -DCHAOS_ROOT=/path/to/chaos-pp-x
cmake --build build
ctest --test-dir build --output-on-failure
```

---

## CMake Integration

Order defines and installs an `INTERFACE` target `order::order` with export headers and include directories:

### Subdirectory / FetchContent
```cmake
add_subdirectory(order-pp-x)
target_link_libraries(my_project PRIVATE order::order)
```

### Package Config (`find_package`)
```cmake
# After installation (cmake --install build)
find_package(order REQUIRED)
target_link_libraries(my_project PRIVATE order::order)
```

---

## Quality Assurance & Bug Fixes

| Area | Status | Notes |
|---|---|---|
| **C99 & C++11 Conformance** | **100% PASS** | Verified with Clang and GCC with `-Wall -Wextra -Wpedantic` |
| **Associative Map ADT** | **Integrated** | Merged PR #6 (`order/map.h`): full dictionary ADT (`insert`, `at`, `erase`, `union`, `diff`) |
| **Tuple Ingestion Bugfix** | **Fixed** | Fixed `ORDER_PP_TOKENS_TO_SEQ_EDIBLE_TAKE` to support comma-separated tuples |
| **Convenience Headers** | **Added** | Added `<order/map.h>` forwarding header for clean `#include` access |

---

## License

Distributed under the [Boost Software License, Version 1.0](LICENSE).
