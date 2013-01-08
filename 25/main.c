#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include <utils/bignum.h>
#include <common.h>

/* Note 
 * 要はbig integerがあれば動的計画法で一発なんだけど、
 * せっかくだから作ってみたら、
 * やっぱり浅はかなので10進数変換するのにてこずる。
 * 早すぎる最適化は良くないですね
 */


int main(int argc, char **argv)
{
    
    for (int c; (c = getopt(argc, argv, "n:")) != -1;) {
        switch(c) {
        }
    }

    struct bignum_uint *big_a = bignum_uint_create(1);
    struct bignum_uint *big_b = bignum_uint_create(1);
    struct bignum_uint *big_c = bignum_uint_create(0);


    int i = 2;

    /*
    bignum_uint_add(big_a, big_b, big_c);
    bignum_uint_dump(big_c);
    */

    uint32_t digits = 0;

    while(digits < 1000) {
        bignum_uint_add(big_a, big_b, big_c);
        bignum_uint_digits(big_c,&digits);
        swap(big_c,big_b);
        swap(big_b,big_a);
        i++;
    }

    DUMPD(i);

    return 0;
}
