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

/* Note:
 * 簡単なので特にコメントなし
 * リファクタはがんばった
 */

DECLARE_VECTOR(int);

static inline bool is_palindromic(uint32_t base,uint32_t x)
{
    uint32_t y = 0;
    uint32_t tmp = x;

    while(tmp > 0) {
        y *= base;
        y += tmp % base; 
        tmp /= base;
    }

    return x == y;
}

int main(int argc, char **argv)
{
    for (int c; (c = getopt(argc, argv, "n:p:")) != -1;) {
        switch(c) {
        }
    }

    int result = 0;

    int i;

    for(i = 0; i < 1000000; i++) {
        if(is_palindromic(10,i) &&
           is_palindromic(2,i)) {
            result += i;
        }
    }

    DUMPD(result); /* 11 is doublly counted */

    return 0;
}
