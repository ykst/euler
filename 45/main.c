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
 * 上界がよくわからん問題が増えてきたが、
 * これについては素直にDPでまあまあいける。
 * intだとラップアラウンドしてしまって処理が終了しなかった。
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

    uint64_t i,j = 1,k = 1;
    uint64_t n2 = 0, n5 = 0, n6;

    for(i = 1; result <= 40755; i++) {
        n6 = i * (2 * i - 1);
        for(; n5 < n6; j++) {
            n5 = j * (3 * j - 1) / 2;
            for(; n2 < n6; k++) {
                n2 = k * (k + 1) / 2;
                if(n6 == n2 && n2 == n5) {
                    printf("T%llu P%llu H%llu: %llu\n", k, j, i, n6);
                    result = n6;
                }
            }
        }
    }

    DUMPL(result); 

    return 0;
}
