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
 * 値の大きいpandigitalをイタレートして、
 * 目的の条件に合致するかどうかで判断。
 * ピボットを境に整数を分割する処理でバグってひとはまり
 */

DECLARE_VECTOR(int);

static bool check_cateach(uint32_t x, int key, int mult)
{
    if(x == 0) {
        return true;
    }

    uint32_t tmp = x;
    uint32_t accum = 0;
    uint32_t mkey = key*mult;
    uint32_t base = 1;

    while(tmp > 0) {
        if(mkey == tmp) {
            return check_cateach(accum, key, mult+1);
        }
        base *= 10;
        accum = x % base;
        tmp /= 10;
    }

    return false;
}

static uint32_t check_catproduct(int result[9])
{
    uint32_t x = 0;
    int i;
    int tmp;
    int accum;

    for(i = 0; i < 9; i++) {
        x *= 10;
        x += result[i];
    }

    tmp = x;
    accum = 0;
    while(tmp > 0) {
        tmp /= 10;
        if(check_cateach(x, tmp, 1)) {
            return x;
        }
    }

    return 0;
}

static void iter_pandigital(int used[9], int result[9], int pivot, uint32_t *p_result)
{
    if(pivot == 9) {
        *p_result = check_catproduct(result);
        return;
    }

    int i;

    for(i = 8; i >= 0; i--) {
        if(!used[i]) {
            result[pivot] = i + 1;
            used[i] = 1;
            iter_pandigital(used, result, pivot + 1, p_result);
            used[i] = 0;
            if(*p_result) return;
        }
    }
}

int main(int argc, char **argv)
{
    for (int c; (c = getopt(argc, argv, "n:p:")) != -1;) {
        switch(c) {
        }
    }

    uint32_t result = 0;
    int used[9] = {};
    int digits[9] = {};

    iter_pandigital(used, digits, 0, &result);

    DUMPD(result); 

    return 0;
}
