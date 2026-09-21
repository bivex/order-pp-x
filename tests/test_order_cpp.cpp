#include <iostream>
#include <cassert>
#include <order/interpreter.h>
#include <order/lib/stream.h>
#include <order/lib/collections/map.h>

int main() {
    // Arithmetic in C++
    int fib = ORDER_PP(8to_lit(8add(5, 7)));
    assert(fib == 12);

    // Map collection size
    int map_sz = ORDER_PP(8to_lit(8map_size(8map(8equal, (1, 2)(8add(2, 3), 10)(8, 20)))));
    assert(map_sz == 3);
    // Streams
    int stream_head_val = ORDER_PP(8to_lit(8stream_head(8stream_of_naturals)));
    assert(stream_head_val == 0);

    // Tokens to seq with multi-element tuples
    int tokens_sz = ORDER_PP(8to_lit(8seq_size(8tokens_to_seq(8quote((10, 20)(30, 40))))));
    assert(tokens_sz == 2);

    std::cout << "Order Metalanguage (C++) comprehensive tests passed successfully!" << std::endl;
    return 0;
}
