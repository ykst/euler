#include <stdlib.h>
#include <stdio.h>

#include <common.h>

/* Note 
 * Haskellでやろうとして、ランダムアクセスがうまくいかないのなんので挫折してしまった問題
 * フィルター作戦でいけるかどうか微妙な線だが、意外と計算量的には対したこと無かった。
 * n^2でもまだ扱える問題領域だっただけということだろう。
 * 最初はfilteredにabundant単体で加えてしまっていたので答えが出なかった。
 */
static inline int is_abundant(int x)
{
    int i;
    int sum = 0;
    for(i = 1; i <= x / 2; i++) {
        if((x % i) == 0) {
            sum += i;
        }
    }

    return sum > x;
}

int main(int argc, char **argv)
{

    int x,y;
    int filtered[28123 + 1] = {};
    int abundant[28123 + 1] = {};

    void *pvoid = NULL;
    uint16_t *p = pvoid;
    pvoid = p;
    p = NULL;

    for(x = 12; x <= 28123; x+=1) {
        if(is_abundant(x)) {
            abundant[x] = 1;
            for(y=12; y <= x; y+=1) {
                if(abundant[y] && (y + x <= 28123)) {
                    filtered[y + x] = 1;
                } 
            }
        }
    }

    int sum = 0;
    for(x = 1; x <= 28123; x++) {
        if(filtered[x] == 0) {
            DUMPD(x);

            sum += x;
        }
    }

    DUMPD(sum);

    return 0;
}
