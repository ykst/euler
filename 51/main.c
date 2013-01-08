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
 * かなりてこずった。まず、二桁だけ変えるのかと思ってやってたら
 * 答えが出なくて素数テーブルを増やしまくったりしてなんとかしようとしてた。
 * だが、そうすると明らかに5秒ルールを突破してしまうのであった
 * そして、問題の中には一言も2桁だけとは書いてなかったので、
 * 任意桁数の置換が可能なように組み替える必要があった。
 * これは再帰を使わないと実装が厳しいのでほとんどのひとが脱落したと思われる。
 * 最初に0となる場所を探して、そこからの足し算で変形を作るところを思いついた時点で
 * すぐに片がつくと思ったのだが。。
 * また、一番上の桁が0になるパターンがどう考えても罠なので、
 * いやはやなかなか厳しい
 */

DECLARE_VECTOR(int);

static int check(int x, int *pivot)
{
    int i = 0;
    int pidx = 0;
    while (x > 0) {
        if(x % 10 == 1) {
            pivot[pidx++] = i;
        }
        x /= 10;
        i++;
    }

    return pidx;
}

static int rec_check(int base, int *pivots, int mlen, int pivot, int ofs, int combs)
{
    if(pivot == mlen) {
        int pcnt = 0;
        int cp;
        int j;
        int minp = INT_MAX;
        if(combs < 2) return 0;

        for(j = 0; j < 10; j++) {
            cp = base + ofs * (j-1);
            if(btable_get(ptable, cp)) {
                ++pcnt;
                minp = min(minp, cp);
            }
        }

        if(pcnt == 8) {
            return minp;
        }

        return 0;
    }

    int ret1, ret2;

    ret1 = rec_check(base, pivots, mlen, pivot+1, ofs + POW(10,pivots[pivot]), combs + 1);
    ret2 = rec_check(base, pivots, mlen, pivot+1, ofs, combs);

    return max(ret1,ret2);
}

int main(int argc, char **argv)
{
    int x_in = 0;

    int d[10]={};
    for (int c; (c = getopt(argc, argv, "n:x:")) != -1;) {
        switch(c) {
            case 'n':
                DUMPD(check(atoi(optarg),d));
                exit(0);
            case 'x':
                x_in = atoi(optarg);
                DUMPD(!!btable_get(ptable,x_in));
                exit(0);
        }
    }

    int result = 0;
    int i, minp = INT_MAX;
    int len;
    int pivot[10] = {};

    for(i = 1000; i < 1000000; i++) {
        if((len = check(i, pivot)) >= 2) {
            if((minp = rec_check(i, pivot, len, 0, 0, 0))) DUMPD(minp);
        }
    }

    DUMPD(result = minp);

    return 0;
}
