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
 * btableをオーバーフローしまくってでたらめな値が出てた。SEGVらないのね。。
 * そして答えが想像以上にでかい値だった。
 */

DECLARE_VECTOR(int);

static inline int pentagonal(int n)
{
    return n * (3 * n - 1) / 2 ;
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
    uint32_t pents[1000000] = {};
    vector(int) *vpents = vector_new(int, 100);

    int i, j, n = 0, m = 0;
    for(i = 1; ; i++) {
        n = pentagonal(i);
        if(n > 32000000) break;
        btable_set(pents, n);
        vector_pushback(vpents, n);
        m = n;
    }

    int cur, tgt, sum, diff;
    uint32_t d = 0xFFFFFFFF;
    for(i = 1; i < vpents->idx - 1; i++) {
        cur = vpents->buf[i];

        if(cur - vpents->buf[i - 1] > d) {
            ICHECK;
            break;
        }

        for(j = 0 ;j < i; j++) {

            tgt = vpents->buf[j];
            sum = cur + tgt;
            diff = cur - tgt;

            //printf("%d %d %d %d\n", tgt, cur, sum, diff);
            if(sum <= n && 
               btable_get(pents, sum) && 
               btable_get(pents, diff)) {

                printf("%d %d %d %d\n", tgt, cur, sum, diff);

                d = min(d, diff);
            }
        }
    }

    vector_delete(vpents);

    DUMPL(result = d); 

    return 0;
}
