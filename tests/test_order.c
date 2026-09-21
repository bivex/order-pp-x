#include <stdio.h>
#include <assert.h>
#include <order/interpreter.h>
#include <order/lib/collections/map.h>

int main(void) {
    /* 1. Arithmetic & Logic */
    int sum = ORDER_PP(8to_lit(8add(2, 3)));
    assert(sum == 5);

    int mul = ORDER_PP(8to_lit(8times(3, 4)));
    assert(mul == 12);

    /* 2. Tuple operations */
    int tuple_sz = ORDER_PP(8to_lit(8tuple_size(8quote((1, 2, 3, 4, 5)))));
    assert(tuple_sz == 5);

    /* 3. Sequences */
    int seq_sz = ORDER_PP(8to_lit(8seq_size(8quote((a)(b)(c)))));
    assert(seq_sz == 3);

    /* 4. Map collection (from PR #6) */
    int map_sz = ORDER_PP(8to_lit(8map_size(8map(8equal, (1, 2)(8add(2, 3), 10)(8, 20)))));
    assert(map_sz == 3);

    printf("Order Metalanguage (C99) smoke tests passed successfully!\n");
    return 0;
}
