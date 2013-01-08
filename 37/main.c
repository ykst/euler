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
 * 間に2が入るのはだめ。
 * 範囲が微妙なので、とりあえず1000000まででやったら間に合ってしまった。
 * 多分これでは甘いので、本来は可変長のハッシュとかを使って素数判定をしないといけないだろう。
 */

DECLARE_VECTOR(int);

static bool rot_check_l(uint32_t *ptable, int x)
{
    x /= 10;
    if(x == 0) return true;

    return btable_get(ptable, x) && rot_check_l(ptable, x);
}

static bool rot_check_r(uint32_t *ptable, int x, int org, int pivot)
{
    x = org % pivot;
    if(x == org) return true;

    return btable_get(ptable, x) && rot_check_r(ptable, x, org, pivot * 10);
}

int main(int argc, char **argv)
{
    for (int c; (c = getopt(argc, argv, "n:p:")) != -1;) {
        switch(c) {
        }
    }

    int result = 0;
    int i;
    uint32_t ltable[1000000/32 + 1] = {};

    btable_set(ltable,2);

    for(i = 3; i < 1000000; i+=2) {
        if(is_prime(i)) {
            btable_set(ltable, i);
        }
    }

    for(i = 11; i < 1000000; i += 2) {
        if(btable_get(ltable,i)) {
            if(rot_check_l(ltable,i) && 
               rot_check_r(ltable,i,i,10)) {
                DUMPD(i);
                result += i;
            }
        }
    }

    DUMPD(result); 

    return 0;
}
