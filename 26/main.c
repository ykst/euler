#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include <common.h>
#include <stdbool.h>

/* Note 
 * 数列の中から同じ長さを持つ部分集合を探す困難な問題と思いきや、
 * 単にdiviserが同じになるかどうかを周期的にチェックすりゃよかった。
 */

static inline int calc_recurring_cycle(bool dump_it,int d)
{
    int rem = 1;
    int div;
    int i = 0,j;
    int *tmp;
    int len = 1000;

    TALLOCS(tmp, len, return FAILURE);

    while (rem != 0) {
        rem *= 10;

        tmp[i] = rem;

        for(j = 0; j < i; j++) {
            if(tmp[j] == rem) {
                FREE(tmp);
                return i - j;
            }
        }

        if(rem < d) {
            if(dump_it) printf("0");
            continue;
        }

        div = rem / d;
        if(dump_it) printf("%d",div);
        rem %= d;

        i++;
        if(i >= len) {
            len *= 2;
            REALLOC(tmp,len, FREE(tmp);return FAILURE);
        }
    }

    FREE(tmp);
    return 0;
}

int main(int argc, char **argv)
{
    for (int c; (c = getopt(argc, argv, "n:")) != -1;) {
        switch(c) {
            case 'n':
                calc_recurring_cycle(1,atoi(optarg));
                break;
        }
    }

    int len = 0;
    int mlen = 0;
    int result = 0;
    int i;
    
    for(i = 1; i < 1000; i++) {
       len = calc_recurring_cycle(0,i);
       if(len > mlen) {
           result = i;
           mlen = len;
       }
    }

    DUMPD(result);

    return 0;
}
