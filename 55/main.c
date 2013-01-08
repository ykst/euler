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
 * 親切にイテレートする回数まで教えてくれてるわけだが、残念ながらbignumになってしまう。
 * stringベースのbignumを作るか、なんなのか。Haskellでやったら一発のような気が。。ワンライナーでは厳しいが。
 * brute forceで片付いてしまうしなぁ。。
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


    int result = 0;

    DUMPD(result);

    return 0;
}
