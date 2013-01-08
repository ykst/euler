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
 * 組み合わせ問題において、先に探索空間を計算するパターンの問題
 * そこに気付けば楽勝
 */

DECLARE_VECTOR(int);

int main(int argc, char **argv)
{
    for (int c; (c = getopt(argc, argv, "n:p:")) != -1;) {
        switch(c) {
        }
    }

    uint32_t result = 0;
    uint32_t triangles[1000] = {};

    int i,j;
    int m = 0;
    int perimeter;

    for(i = 1; i < 500; i++) {
        for(j = i; j < 1000; j++) {
            uint32_t dd = i*i + j*j;
            uint32_t t = sqrt(dd);
            if(dd % t == 0) {
                perimeter = i + j + t;
                if(perimeter <= 1000) {
                    if(++triangles[perimeter] > m) {
                        m = triangles[perimeter];
                        result = perimeter;
                    }
                }
            }
        }
    }

    DUMPD(result); 

    return 0;
}
