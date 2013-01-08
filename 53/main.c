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
 * 完全なるやるだけ問題だが、bignumの無い我々としては途中でgcdで数を減らすとかいうしょうもない処理をしないといけない
 */

DECLARE_VECTOR(int);


int main(int argc, char **argv)
{
    int x_in = 0;

    for (int c; (c = getopt(argc, argv, "n:x:")) != -1;) {
        switch(c) {
            case 'n':
                DUMPD(comb(5,atoi(optarg)));
                exit(0);
            case 'x':
                x_in = atoi(optarg);
                exit(0);
        }
    }

    int result = 0;
    int n,r;

    for(n = 1; n <= 100; n++) {
        for(r = 1; r <= n; r++) {
            if(comb(n,r) > 1000000) {
                result +=  n - 2 * r + 1;
                break;
            }
        }
    }

    DUMPD(result);

    return 0;
}
