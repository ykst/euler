#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <math.h>

#include <common.h>
#include <stdbool.h>

/* Note:
 * 問題の意味がよくわからないシリーズだが、単に試すだけでいけた。
 * いんちきvector導入
 */

#define DECLARE_VECTOR(t) struct vector_##t {t *buf; size_t len; size_t idx;}

#define VECTOR(t) struct vector_##t

#define VECTOR_NEW(t, s) NEW(VECTOR(t),.buf = calloc(s,sizeof(t)), .len = (s), .idx = 0)

#define VECTOR_DELETE(p) ({if(p) {FREE((p)->buf); FREE(p)} })

#define VECTOR_MULTIPLY(p) ({typeof(*(p)->buf) ___vector_multiply_r = 1; \
        for( int ___vector_multiply_i = 0; ___vector_multiply_i < (p)->idx; ___vector_multiply_i++) { \
        ___vector_multiply_r *= (p)->buf[___vector_multiply_i]; \
        } \
        ___vector_multiply_r; })

#define VECTOR_PUSHBACK(p, e) ({\
    if((p)->idx >= (p)->len) {\
        p->len *= 2; \
        REALLOC((p)->buf, (p)->len, ERR("realloc failed\n");exit(1)); \
        for(int _____i = (p)->idx; _____i < (p)->len; _____i++) {\
            (p)->buf[_____i] = 0; \
        } \
    } \
    (p)->buf[(p)->idx++] = (e); })

DECLARE_VECTOR(int);

static void lct(int n,int d, int *on, int *od)
{
    int i;
    int m = min(n,d);
    *on = n;
    *od = d;

    if(abs(d) <= 1) return;

    i = 2;
    while(i <= m) {
        if((n % i) == 0 &&
           (d % i) == 0) {
            n /= i;
            d /= i;
            m = min(n, d);
            *on = n;
            *od = d;
            i = 2;
        } else {
            ++i;
        }
    }
}


int main(int argc, char **argv)
{
    for (int c; (c = getopt(argc, argv, "n:p:")) != -1;) {
        switch(c) {
        }
    }

    int result = 0;
    VECTOR(int) *ns= VECTOR_NEW(int, 100);
    VECTOR(int) *ds= VECTOR_NEW(int, 100);


    int i,j;
    for(i = 10; i < 99; i++) {
        for(j = i + 1; j < 100; j++) {
            int n,d, wn, wd;
            lct(i,j,&n,&d);

            if (i % 10 == j / 10)  {
                if(i / 10 < j % 10) {
                    lct(i / 10, j % 10, &wn, &wd);
                    if(n == wn && d == wd) {
                        printf("%d/%d = %d/%d\n", i , j , n, d);
                        printf("%d/%d = %d/%d\n", i / 10, j % 10, wn, wd);
                        VECTOR_PUSHBACK(ns, i); 
                        VECTOR_PUSHBACK(ds, j); 
                    }
                }
            }
        }
    }

    int dummy;

    lct(VECTOR_MULTIPLY(ns), VECTOR_MULTIPLY(ds), &dummy, &result);
    DUMPD(result);

    VECTOR_DELETE(ns);
    VECTOR_DELETE(ds);

    return 0;
}
