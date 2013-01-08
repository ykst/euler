#define __DEBUG
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
#include <utils/fileop.h>

#include <gmp.h>

/* Note:
 * 前の奴はHaskellでダラダラやったが今回はC。しかし、やっぱりというか速いっすね
 */

DECLARE_VECTOR(int);

int main(int argc, char **argv)
{
    for (int c; (c = getopt(argc, argv, "n:x:")) != -1;) {
        switch(c) {
            case 'n':
                exit(0);
            case 'x':
                exit(0);
        }
    }

    int result = 972;

    FILE *fp = NULL;
    ASSERT(fp = FOPEN("r", "triangle.txt"), exit(1));

    int tri[100][1000] = {};
    int i = 0;
    char buf[1024];
    char *ptr;
    char *word;

    while(fgets(buf, 1024, fp)) {
        ptr = buf;
        int j = 0;
        while((word = strtok(ptr, " "))) {
            sscanf(word, "%d", &tri[i][j]);
            ++j;
            ptr = NULL;
        }
        tri[i][j] = -1;
        ++i;
    }

    int tmp[1000] = {};

    for(i -= 1 ;i >= 0; i--) {
        int j = 0;
        while(tri[i][j] >= 0) {
            tmp[j] = max(tmp[j], tmp[j+1]);
            tmp[j] += tri[i][j];
            ++j;
        }
    }

    result = tmp[0];

    DUMPD(result);

    return 0;
}
