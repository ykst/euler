#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <math.h>
#include <stdint.h>

#include <common.h>
#include <stdbool.h>

/* Note:
 * ビットテーブルを導入。ローテートして同じになるケースをチェックしていないが、いまは11しか無いので大丈夫な感じになってしまっている。
 */
int fac_array[10] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880};

#define DECLARE_VECTOR(t) struct vector_##t {t *buf; size_t len; size_t idx;}

#define VECTOR(t) struct vector_##t

#define VECTOR_NEW(t, s) NEW(VECTOR(t),.buf = calloc(s,sizeof(t)), .len = (s), .idx = 0)

#define VECTOR_DELETE(p) ({if(p) {FREE((p)->buf); FREE(p)} })

#define VECTOR_MULTIPLY(p) ({typeof(*(p)->buf) ___vector_multiply_r = 1; \
        for( int ___vector_multiply_i = 0; ___vector_multiply_i < (p)->idx; ___vector_multiply_i++) { \
        ___vector_multiply_r *= (p)->buf[___vector_multiply_i]; \
        } \
        ___vector_multiply_r; })

#define VECTOR_SUM(p) ({typeof(*(p)->buf) ___vector_sum_r = 0; \
        for( int ___vector_sum_i = 0; ___vector_sum_i < (p)->idx; ___vector_sum_i++) { \
        ___vector_sum_r += (p)->buf[___vector_sum_i]; \
        } \
        ___vector_sum_r; })

#define VECTOR_PUSHBACK(p, e) ({\
    if((p)->idx >= (p)->len) {\
        p->len *= 2; \
        REALLOC((p)->buf, (p)->len, ERR("realloc failed\n");exit(1)); \
        for(int _____i = (p)->idx; _____i < (p)->len; _____i++) {\
            (p)->buf[_____i] = 0; \
        } \
    } \
    (p)->buf[(p)->idx++] = (e); })

#define POW(x,i) ({typeof(x) ____pow_result = 1;for(int ____pow_i = 0; ____pow_i < (i); ____pow_i++) ____pow_result *= (x); ____pow_result;})

static inline bool is_prime(int x)
{
    int limit = sqrt(x);
    int i;

    if(x <= 1) return false;

    if((x & 1) || x == 2 || x == 3) {
        for(i = 3; i  <= limit; i += 2) {
            if((x % i) == 0) {
                return false;
            }
        }
        return true;
    }

    return false;
}

DECLARE_VECTOR(int);

#define BTABLE_SET(a,i) ((a)[(i)/32] |= (1 << ((i) % 32)))
#define BTABLE_GET(a,i) ((a)[(i)/32] & (1 << ((i) % 32)))
#define BTABLE_CLR(a,i) ((a)[(i)/32] &= ~(1 << ((i) % 32)))

static inline int rotate(int num, int digits,int rot)
{
    return num = POW(10, digits - rot)* (num % POW(10,rot)) + num / POW(10, rot);
}

int main(int argc, char **argv)
{
    for (int c; (c = getopt(argc, argv, "n:p:")) != -1;) {
        switch(c) {
        }
    }

    uint32_t ltable[1000000/32 + 1] = {};

    int result = 0;

    int i;

    for(i = 0; i < 1000000; i++) {
        if(is_prime(i)) {
            BTABLE_SET(ltable, i);
        }
    }

    int digits;
    int range = 1;
    int r;

    for(digits = 1; digits < 7; digits++) {
        for(i = range; i < range * 10; i++) {
            if(BTABLE_GET(ltable, i)) {
                bool ok = true;
                for(r = 1; r < digits; r++) {
                    if(!BTABLE_GET(ltable,rotate(i,digits,r))) {
                        ok = false;
                        break;
                    }
                }
                if(ok) {
                    result += digits;
                    for(r = 0; r < digits; r++) {
                        int rotprime;
                        rotprime = rotate(i,digits,r);
                        DUMPD(rotprime);
                        BTABLE_CLR(ltable,rotprime);
                    }
                }
            }
        }
        range *= 10;
    }

    DUMPD(result - 1); /* 11 is doublly counted */

    return 0;
}
