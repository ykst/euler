#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include <common.h>

/* Note 
 * 始めは変換方法がビットで符号化出来るという方針でやったが、
 * 問題の特性がうまくつかめなかった。
 * permutationしてsortするとかいうやり方をやらなくなっただけでも成長か。
 * 実際には探す数 * 次元数までしか同時にはテンポラリバッファは必要ないので、
 * 再帰でビビってmallocする必要は無かった。
 * 逆順でやる。
 */

static void swap_iter(char *tgt, int dim, int pos, int *p_cnt)
{

    if(pos <= 1) {
        swap(tgt[0], tgt[1]);
        ++(*p_cnt);
        return;
    }

    char *tmp;
    int i,j;

    TALLOCS(tmp,dim, exit(1));
    memcpy(tmp, tgt, dim);

    i = 0;
    do {
        if(i != 0) {
            memcpy(tgt, tmp, dim);
            char c = tgt[pos - i];
            for(j = pos - i; j < pos; j++) {
                tgt[j] = tgt[j + 1];
            }
            tgt[pos] = c;
            
            ++(*p_cnt);
            if(*p_cnt >= 1000000) return;
        }
        swap_iter(tgt, dim, pos -1, p_cnt);
        if(*p_cnt >= 1000000) return;
        i+=1;
    } while (i <= pos );

    FREE(tmp);

    return;
}

int main(int argc, char **argv)
{
    size_t dim = 4;
    char *lex;
    
    for (int c; (c = getopt(argc, argv, "n:")) != -1;) {
        switch(c) {
            case 'n': dim= atoi(optarg); break;
        }
    }

    TALLOCS(lex,dim,exit(1));
    
    int i;
    for(i = 0; i < dim; i++) {
        lex[i] = '0' + dim - i - 1;
    }

    i = 1;

    swap_iter(lex, dim, dim-1, &i);

    DUMPD(i);
    DUMPS(lex);

    return 0;
}
