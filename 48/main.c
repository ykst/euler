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
 * bignumがあれば一瞬で終わってしまうので、実際これはHaskellで解いた。
 * しかし、本質はそこではないだろう。
 * お手製のbignumでとりあえず足し算でやってみる。
 * しかし、当然計算量が爆発、やはりbignumの掛け算は難しい
 * 再発明してはいけない部分か、別の道を探る必要があるだろう。。
 * そして彼らは僕をgmpに導いてくれた
 * dumbmpというポータブル版の実装はいわゆるナイーブな初手をそのまま実装している
 * たとえば、mulだとhiとloに分けて、それぞれで計算をやるみたいな感じね。
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
    unsigned int i ;
    mpz_t nm,rez;
    mpz_init(nm);
    mpz_init(rez);

    mpz_set_ui(rez,0);

    for (i = 1; i <= 1000; i++) {
        mpz_set_ui(nm, i);
        mpz_pow_ui(nm,nm, i);
        mpz_add(rez,rez,nm);
    }
    gmp_printf("%Zd\n" , rez); 



    DUMPL(result); 

    return 0;
}
