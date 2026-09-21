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

    /* 4. Map collection (PR #6) and operations */
    int map_sz = ORDER_PP(8to_lit(8map_size(8map(8equal, (1, 2)(8add(2, 3), 10)(8, 20)))));
    assert(map_sz == 3);

    int map_at_val = ORDER_PP(8lets(
        (8M, 8map(8equal, (1, 10)(2, 20))),
        8to_lit(8map_at(2, 8M))
    ));
    assert(map_at_val == 20);

    int map_has = ORDER_PP(8lets(
        (8M, 8map(8equal, (1, 10)(2, 20))),
        8to_lit(8if(8map_exists(1, 8M), 1, 0))
    ));
    assert(map_has == 1);

    int map_ins_sz = ORDER_PP(8lets(
        (8M, 8map(8equal, (1, 10)(2, 20)))
        (8N, 8map_insert(3, 30, 8M)),
        8to_lit(8map_size(8N))
    ));
    assert(map_ins_sz == 3);

    int map_erase_sz = ORDER_PP(8lets(
        (8M, 8map(8equal, (1, 10)(2, 20)))
        (8N, 8map_erase(1, 8M)),
        8to_lit(8map_size(8N))
    ));
    assert(map_erase_sz == 1);

    int map_union_sz = ORDER_PP(8lets(
        (8M, 8map(8equal, (1, 10)))
        (8N, 8map(8equal, (2, 20))),
        8to_lit(8map_size(8map_union(8M, 8N)))
    ));
    assert(map_union_sz == 2);

    int map_intersect_sz = ORDER_PP(8lets(
        (8M, 8map(8equal, (1, 10)(2, 20)))
        (8N, 8map(8equal, (2, 22)(3, 30))),
        8to_lit(8map_size(8map_intersect(8M, 8N)))
    ));
    assert(map_intersect_sz == 1);

    /* 5. Tokens to seq bugfix (handling multi-element tuples with commas) */
    int tokens_to_seq_sz = ORDER_PP(8to_lit(8seq_size(8tokens_to_seq(8quote((1, 2)(3, 4))))));
    assert(tokens_to_seq_sz == 2);

    printf("Order Metalanguage (C99) comprehensive tests passed successfully!\n");
    return 0;
}
