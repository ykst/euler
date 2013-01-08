#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <math.h>

#include <common.h>
#include <stdbool.h>

/* Note:
 * Big integerを用意する問題では無いことはすぐに分かったが、
 * 動的メモリの構造体を確保するのにまごつく。
 * やってることは完全にvector<>なので、そろそろc++に行った方がいいんだろうか。。
 */

struct mpair {
    int *buf;
    int area;
};

static inline int mpair_init(struct mpair *p) 
{
    p->area = 1000;
    TALLOCS(p->buf, p->area, return FAILURE);

    return SUCCESS;
}

static inline void mpair_clear(struct mpair *p) 
{
    FREE(p->buf);
}

static inline int mpair_add(struct mpair *p, int x)
{
    if(x >= p->area) {
        REALLOC(p->buf, p->area * 2, return FAILURE);
        CLEAR_ARRAY(&p->buf[p->area], p->area);
        p->area *= 2;
    }

    p->buf[x] = 1;

    return SUCCESS;
}

static inline int mpair_count(struct mpair *p)
{
    int i;
    int accum = 0;
    for(i = 0; i < p->area; i++) {
        if(p->buf[i]) accum++;
    }
    return accum;
}

static inline void calc_mpair(int a, int b, int *p_mi, int *p_mj)
{
    int i,j,t;

    int limit = sqrt(a);

    for( i = 2; i <= limit; i++) {
        t = i * i;

        for(j = 2; t <= a; j++) {
            if(a == t) {
                *p_mi = i;
                *p_mj = b * j;

                return;
            }

            t *= i;
        }
    }

    *p_mi = a;
    *p_mj = b;
}

int main(int argc, char **argv)
{
    int t1, t2, t3, t4;

    for (int c; (c = getopt(argc, argv, "n:p:")) != -1;) {
        switch(c) {
            case 'n':
                ASSERT(sscanf(optarg, "%d^%d", &t1, &t2) == 2, exit(1));
                calc_mpair(t1, t2, &t3, &t4);
                DUMPD(t3);
                DUMPD(t4);
                exit(0);
        }
    }

    struct mpair mpairs[101] = {};
    int i,j;
    int mi,mj;
    int result;

    for(i = 2; i <= 100; i++) {
        mpair_init(&mpairs[i]);
    }

    for(i = 2; i <= 100; i++) {
        for(j = 2; j <= 100; j++) {
            calc_mpair(i,j,&mi,&mj);
            mpair_add(&mpairs[mi], mj);
        }
    }

    result = 0;
    for(i = 2; i <= 100; i++) {
        result += mpair_count(&mpairs[i]);
        mpair_clear(&mpairs[i]);
    }

    DUMPD(result);

    return 0;
}
