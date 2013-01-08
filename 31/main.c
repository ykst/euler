#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <math.h>

#include <common.h>
#include <stdbool.h>

/* Note:
 * 欲張り方+ 再帰で一発。再帰の深さは高々種類数+200枚で、引数以外のスタックも要らない
 */


static inline int pow5(int x)
{
    return x * x * x * x * x;
}

static void greed_search(int pennydef[8], int pivot, int accum, int *p_result)
{
    if(pivot >= 8) return;
    if(accum == 200) {
        *p_result += 1;
        return;
    }

    if(accum > 200) {
        return;
    }

    greed_search(pennydef, pivot, pennydef[pivot] + accum, p_result);
    greed_search(pennydef, pivot + 1, accum, p_result);
}

int main(int argc, char **argv)
{

    for (int c; (c = getopt(argc, argv, "n:p:")) != -1;) {
        switch(c) {
        }
    }

    int pennydef[8] = {200,100,50,20,10,5,2,1};
    int result = 0;

    greed_search(pennydef, 0, 0, &result);

    DUMPD(result);

    return 0;
}
