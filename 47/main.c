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

/* Note:
 * 題意を解釈するために、まずは例示されたケースでデバッグするという大切さ
 * 3秒ぐらいかかってるのでダメダメ。まず異なるprimesを使ってるかどうかでallocaを使ってるが、
 * これなんとかならんか。
 * 小手先を弄しただけでは改善しなかったが、
 * スレッドを見てみたら、最初に組み合わせ的にブーリアンテーブルを埋め尽くして、
 * 最初に4つ続いてるやつのインデックスを取るとかあった。
 * なるほど。つまり、factorizationはひどく時間が掛かるということだな。
 * 毎回新しい値をfactorizeするということは前の計算結果を全く使えない話になるし、
 * まあ割り算も使うし。ふむふむ
 */

DECLARE_VECTOR(int);

static inline bool check(uint32_t *ptable, uint32_t x, uint64_t *p_k)
{
    int k;
    int n = 0;
    bool ok = false;

    *p_k = 0;

    for(k = 1; ; k++) {
        n = 2 * k * k;
        if(n > x) break;
        if(btable_get(ptable, x - n)) {
            ok = true;
            *p_k = k;
            break;
        }
    }

    return ok;
}

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

    int i,j,k;
    uint32_t ptable[1000000/32 + 1] = {};

    btable_set(ptable, 2);

    int cidx = 0;
    int chain = 0;

    for(i = 3; i < 1000000; i++) {
        if(is_prime(i)) {
            btable_set(ptable, i);
            cidx = 0;
        }  else {
            cidx += 1;
            if(cidx >= 4) {
                int ok = false;
                uint32_t maps[(chain + 3)/32];

                chain = i - 3;

                memset(maps,0,sizeof(int) * (chain + 3) / 32);

                for(j = 0; j < 4; j++) {
                    int tmp = chain + j;
                    int facs = 1;
                    int divs = 0;

                    for(k = 2; k <= tmp; k++) {
                        if(btable_get(ptable,k)) {
                            while((tmp % k) == 0) {
                                facs *= k;
                                tmp /= k;
                            }
                            if(facs != 1) {
                                divs++;
                                if(btable_get(maps,facs)) {
                                    ok = false;
                                    break;
                                }
                                btable_set(maps,facs);
                                facs = 1;
                            }
                        }

                        if(tmp == 1) {
                            ok = (divs == 4);
                            break;
                        } 
                    }
                    if(!ok) break;
                }

                if(ok) {
                    printf("%d %d %d %d\n", chain, chain + 1, chain + 2, chain + 3);
                    result = chain;
                    break;
                }

                cidx = 3;
            }
        }
    }

    DUMPL(result); 

    return 0;
}
