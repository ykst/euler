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

/* Note:
 * いがいとBFでいけちゃったシリーズになってしまった。
 * 今回から計算済みのprimetableを使うように変更
 */

DECLARE_VECTOR(int);


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
    uint32_t ptable[1000000/32] = {};

    btable_set(ptable,2);
    int i;

    for(i = 3; i < 1000000; i+= 2) {
        if(is_prime(i)) {
            btable_set(ptable,i);
        }
    }

    int msum = 0;
    int mprime = 2;
    int mlen = 1;
    int midx = 1;
    int j;
    for(i = 2; i < 1000000; i++) {
        if(btable_get(ptable,i)) {
            msum = i;
            midx = 1;
            for(j = i+1 ; j < 1000000; j++) {
                if(msum + j > 1000000) break;
                if(btable_get(ptable,j)) {
                    msum += j;
                    midx++;
                    if(btable_get(ptable,msum) && midx > mlen) {
                       mprime = msum;
                       mlen = midx;
                    }
                }
           }
        }
    }

    DUMPD(mlen);
    DUMPL(result=mprime); 

    return 0;
}
