#define __DEBUG
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
#include <utils/fileop.h>

#include <gmp.h>

/* Note:
 * bignum系はhaskellでお手軽に答えがわかってしまうからなぁ
 * まじめにやるなら、
 * 今回の問題は全ての指数計算をまじめに計算する必要があるので、
 * おそらくstringでがんばるという感じだろう。
 * ただし累乗の計算は。。
 * やるなら、因数分解か？
 * 例えば53^99とかは、(50 + 3)^99..
 * メモも使えそうだ。んだが、結局はintの幅を超えてしまうので、bignum的なものを作らないといけない。
 * まあ、gmpもあるし、あんまりモチベーションは出ない..
 */

DECLARE_VECTOR(int);

int main(int argc, char **argv)
{
    for (int c; (c = getopt(argc, argv, "n:x:")) != -1;) {
        switch(c) {
            case 'n':
                exit(0);
            case 'x':
                exit(0);
        }
    }

    int result = 972;

    DUMPD(result);

    return 0;
}
