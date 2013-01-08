#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <math.h>

#include <common.h>
#include <stdbool.h>

/* Note:
 * 9! * 9C2 で回せば行けると見て実装。
 * 組み合わせをごりごり計算するだけで終了
 */


struct mbuf
{
    int *buf;
    int len;
    int idx;
} answers;

static void iter_combination(int result[9], int *p_result)
{
    int i, j, k, lhs, rhs, tgt, duplicated;
    for(i = 1; i < 7; i++) {
        for(j = 2; j < 8; j++) {
            lhs = 0;
            rhs = 0;
            tgt = 0;
            for(k = 0; k < i; k++) {
                lhs *= 10;
                lhs += result[k];
            }
            for(k = i; k < j; k++) {
                rhs *= 10;
                rhs += result[k];
            }
            for(k = j; k < 9; k++) {
                tgt *= 10;
                tgt += result[k];
            }

            if(lhs * rhs == tgt) {
                duplicated = 0;
                for(k = 0; k < answers.idx; k++) {
                    if(tgt == answers.buf[k]) {
                        duplicated = 1;
                        break;
                    }
                }

                if(!duplicated) {
                    if(answers.idx >= answers.len) {
                        answers.len *= 2;
                        REALLOC(answers.buf, answers.len, exit(1));
                    }
                    answers.buf[answers.idx++] = tgt;
                    printf("%d * %d = %d\n",lhs, rhs, tgt);
                    *p_result += tgt;
                }
            }
        }
    }
}

static void iter_sequence(int used[9], int result[9], int pivot, int *p_result)
{
    if(pivot >= 9) {
        iter_combination(result, p_result);
        return;
    }

    int i;
    for(i = 0; i < 9; i++) {
        if(used[i] == 0) {
            used[i] = 1;
            result[pivot] = i + 1;
            iter_sequence(used, result, pivot + 1, p_result);
            used[i] = 0;
        }
    }
}

int main(int argc, char **argv)
{
    for (int c; (c = getopt(argc, argv, "n:p:")) != -1;) {
        switch(c) {
        }
    }

    int used[9] = {};
    int result[9] = {};
    int accum = 0;

    answers.len = 1000;
    TALLOCS(answers.buf, answers.len, exit(1));
    answers.idx = 0;

    iter_sequence(used,result,0,&accum);

    DUMPD(accum);

    return 0;
}
