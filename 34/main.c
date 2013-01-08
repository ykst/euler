#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <math.h>

#include <common.h>
#include <stdbool.h>

/* Note:
 * これも上界を求める問題。0!が1であることを忘れると大変に痛い目に遭いますな
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

DECLARE_VECTOR(int);

int main(int argc, char **argv)
{
    for (int c; (c = getopt(argc, argv, "n:p:")) != -1;) {
        switch(c) {
        }
    }

    int result = 0;
    VECTOR(int) *vresult= VECTOR_NEW(int, 100);

    int i,j;

    i = 1;
    j = 10;
    while(1) {
        if(fac_array[8] * i < i * j) break;
        j *= 10;
        i += 1;
    }

    for(i = 3; i < j; i++) {
        int sum = 0;
        int tmp = i;
        while(tmp != 0) {
            sum += fac_array[(tmp % 10)];
            tmp /= 10;
        }
        if(sum == i) {
            DUMPD(i);
            VECTOR_PUSHBACK(vresult, i);
        }
    }
    
    DUMPD(result = VECTOR_SUM(vresult));
    VECTOR_DELETE(vresult);

    return 0;
}
