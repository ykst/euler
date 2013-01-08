#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <math.h>

#include <common.h>
#include <stdbool.h>

/* Note:
 * 明らかにこれは探索範囲の上界を求める問題である。
 * これは右辺の値が9^5のオーダーで収まることから、
 * 探索値の上限も10^6のオーダーであるので、計算量的には体当たりで問題ない
 */


static inline int pow5(int x)
{
    return x * x * x * x * x;
}

int main(int argc, char **argv)
{

    for (int c; (c = getopt(argc, argv, "n:p:")) != -1;) {
        switch(c) {
        }
    }

    int i,j;
    int result = 0;
    int accum = 0;


    for(i = 2; i < 1000000; i++) {
        accum = 0;
        j = i;
        while(j > 0) {
            accum += pow5(j % 10);
            j /= 10;
        }
        if(accum == i) {
            DUMPD(i);
            result += i;
        }
        
    }

    DUMPD(result);

    return 0;
}
