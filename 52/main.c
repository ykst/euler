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

#include <gmp.h>
#include "ptable.c"

/* Note:
 * 組み合わせを求めるのではなく、正規化してbinで処理するというパターン再び。
 * qsortでやるにしても一回の効率が悪いが、
 * たまたま答えが近くにいたということでたいした計算量ではなくなってしまっているのが課題を残すところ
 */

DECLARE_VECTOR(int);

int compare_int(const void *a, const void *b)
{
    return *(int*)a - *(int*)b;
}

static int normalize(int x)
{
    int tmp[16];
    int i = 0, len;
    while(x > 0) {
        tmp[i++] = x % 10;
        x /= 10;
    }

    len = i;

    qsort(tmp, len, sizeof(int), compare_int);

    x = 0;
    for(i = 0; i < len; i++) {
        x *= 10;
        x += tmp[i];
    }

    return x;
}

int main(int argc, char **argv)
{
    int x_in = 0;

    for (int c; (c = getopt(argc, argv, "n:x:")) != -1;) {
        switch(c) {
            case 'n':
                DUMPD(normalize(atoi(optarg)));
                exit(0);
            case 'x':
                x_in = atoi(optarg);
                DUMPD(!!btable_get(ptable,x_in));
                exit(0);
        }
    }

    int result = 0;
    int norm;
    int j;

    for(int i = 100000; i < 1000000; i++) {
        norm = normalize(i); 

        for(j = 1; j < 6; j++) {
            if(normalize(i * (j+1)) != norm) {
                break;
            }
        }

        if(j == 6) {
            result = i;
            break;
        }
    }

    DUMPD(result);

    return 0;
}
