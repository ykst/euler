#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

#include <common.h>
#include <utils/vector.h>
#include <utils/euler.h>
#include <utils/btable.h>
#include <utils/bignum.h>

/* Note:
 * 10000くらいの範囲に運良くあれば大丈夫。
 * しかし、こういうやり方ではいずれ行き詰まるだろう。
 * 2乗して引き算した結果が素数であるかどうかを判定するようにすれば、スピードとしてはベストか？
 */

DECLARE_VECTOR(int);

static inline bool check(uint32_t *ptable, uint32_t x, uint64_t *p_k)
{
    int k;
    int n = 0;
    bool ok = false;

    *p_k = 0;

    for(k = 1; ; k++) {
        n = 2 * k * k;
        if(n > x) break;
        if(btable_get(ptable, x - n)) {
            ok = true;
            *p_k = k;
            break;
        }
    }

    return ok;
}

int main(int argc, char **argv)
{
    int n_in = 0;
    int x_in = 0;

    for (int c; (c = getopt(argc, argv, "n:x:")) != -1;) {
        switch(c) {
            case 'n':
                n_in = atoi(optarg);
                break;
            case 'x':
                x_in = atoi(optarg);
                break;
        }
    }

    uint64_t result = 0;

    uint64_t i,k;
    uint32_t ptable[10000] = {};

    btable_set(ptable, 2);

    for(i = 3; i < 10000 * 32; i+=2) {
        if(is_prime(i)) {
            btable_set(ptable, i);
        } else {
            if(check(ptable, i, &k)) {
                printf("%llu = %llu + 2 * %llu^2\n", i, i - 2 * k * k, k);
            } else {
                result = i;
                break;
            }
        }
    }

    DUMPL(result); 

    return 0;
}
