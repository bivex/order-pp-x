#include <iostream>
#include <cassert>
#include <order/interpreter.h>
#include <order/lib/collections/map.h>

int main() {
    // Arithmetic in C++
    int fib = ORDER_PP(8to_lit(8add(5, 7)));
    assert(fib == 12);

    // Map collection size
    int map_sz = ORDER_PP(8to_lit(8map_size(8map(8equal, (1, 2)(8add(2, 3), 10)(8, 20)))));
    assert(map_sz == 3);

    std::cout << "Order Metalanguage (C++) smoke tests passed successfully!" << std::endl;
    return 0;
}
