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
 * permutationをイタレートするのではなく、ソートして正規化したbinに貯める作戦を思いついただけでも成長か。
 * ソートでミスってひと嵌り。
 * 題意は3330の差があるものに限定していたらしかったが、読み取り損ねてひとはまり
 */

DECLARE_VECTOR(int);

static inline int norm_4digit(uint32_t x)
{
    uint32_t a0, a1, a2, a3;
    a0 = (x / 1000) % 10;
    a1 = (x / 100) % 10;
    a2 = (x / 10) % 10;
    a3 = x % 10;

    if(a0 > a1) swap(a0, a1);
    if(a2 > a3) swap(a2, a3);
    if(a0 > a2) swap(a0, a2);
    if(a1 > a3) swap(a1, a3);
    if(a1 > a2) swap(a1, a2);

    return a0 * 1000 + a1 * 100 + a2 * 10 + a3;
}

int main(int argc, char **argv)
{
    int n_in = 0;
    int x_in = 0;

    for (int c; (c = getopt(argc, argv, "n:x:")) != -1;) {
        switch(c) {
            case 'n':
                n_in = atoi(optarg);
                DUMPD(norm_4digit(n_in));
                exit(1);
                break;
            case 'x':
                x_in = atoi(optarg);
                break;
        }
    }

    uint64_t result = 0;
    uint32_t rtable[10000][32] = {};
    uint32_t otable[10000] = {};

    int i, norm;
    for(i = 1001; i < 10000; i+= 2) {
        if(is_prime(i)) {
            norm = norm_4digit(i);
            rtable[norm][rtable[norm][0]+1] = i;
            rtable[norm][0] += 1;
        }
    }

    int cnt = 0;
    for(i = 1001; i < 10000; i+= 2) {
        int lim = rtable[i][0];
        if(lim >= 3) {
            int j, k;
            int mcnt = 0;
            int midx = 1000000;
            int idx;
            /* 使用済みか否かを調べるために80kB使うのはいかがなものか */
            CLEAR_ARRAY(otable, 10000)
            for(j = 0; j < lim - 1; j++) {
                for(k = j + 1; k < lim; k ++) {
                    idx = rtable[i][k+1] - rtable[i][j+1];
                    otable[idx]++;
                    mcnt = max(otable[idx], mcnt);
                    midx = idx;
                }
            }

            if(midx == 3330) {
                DUMPD(midx);
                for(j = 0; j < lim; j++) {
                    DUMPD(rtable[i][j+1]);
                }
            }
            cnt ++;
        }
    }

    DUMPD(cnt);

    DUMPL(result); 

    return 0;
}
