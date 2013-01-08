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
 * ひたすらやるだけシリーズ。くじけそうになるので、気合いで片付ける。
 * 賞味2時間くらいはかかってるのが悲しいね。
 * 内部のデータ形式として配列が使えるといろいろとショートカット出来て、
 * この辺りはHaskellで不自由しながら最適化するのに比べて気が楽だ
 * だが、Haskellならもっと短く美しく書けるだろう
 * ハイカードの比較にペアの数だけ位数をつけるぐらいが見所ですな。
 * いろいろと高速化の余地はあるが、首を突っ込みすぎてもしょうがない
 */

DECLARE_VECTOR(int);

static int decode(char num, char type)
{
    int fac = 0;
    int n = 0;
    switch(type) {
        case 'C': fac = 0; break;
        case 'H': fac = 1; break;
        case 'D': fac = 2; break;
        case 'S': fac = 3; break;
        default: abort();
    }
    
    switch(num) {
        case 'T': n = 8; break;
        case 'J': n = 9; break;
        case 'Q': n = 10; break;
        case 'K': n = 11; break;
        case 'A': n = 12; break;
        default: n = num - '2';
    }

    return  fac * 13 + n;
}

static void getpairs(int *p, int pairs[13])
{
    int i;

    for(i = 0; i < 5; i++) {
        pairs[p[i] % 13]++;
    }
}

static void gettypes(int *p, int types[4])
{
    int i;

    for(i = 0; i < 5; i++) {
        types[p[i] / 13]++;
    }
}

static void getnums(int *p, int nums[5])
{
    int i;

    for(i = 0; i < 5; i++) {
        nums[i] = p[i] % 13;
    }
    qsort(nums, 5, sizeof(int), compare_int);
}

static bool CONST is_straight(int nums[5])
{
    int i;
    int base = nums[0] % 13;
    int seqs = 0;

    for(i = 1;i < 5; i++) {
        if(nums[i] == ((base + 1) % 13)) {
            seqs++;
        }
        base = (base + 1) % 13;
    }

    if(((nums[4] + 1) % 13) == (nums[0] % 13)) seqs++;

    return seqs == 4;
}

static inline bool CONST high_card(int nums1[5], int pairs1[13], int nums2[5], int pairs2[13])
{
    int i,p;
    int tmp1[13] = {};
    int tmp2[13] = {};

    for(i = 0; i < 5; i++) {
        p = nums1[i];
        tmp1[p] = POW(52, pairs1[p]) * p;
        p = nums2[i];
        tmp2[p] = POW(52, pairs2[p]) * p;
    }

    qsort(tmp1, 13, sizeof(int), compare_int_down);
    qsort(tmp2, 13, sizeof(int), compare_int_down);

    for(i = 0; i < 13; i++) {
        if(tmp1[i] == tmp2[i]) continue;

        if(tmp1[i] > tmp2[i]) {
            return true;
        } 

        return false;
    }
    return false;
}

static inline bool CONST is_npair(int num,int pairs[13])
{
    int i;
    int m = 0;

    for(i = 0; i < 13; i++) {
        if(pairs[i] == 2) {
            m++;
        } 
    }

    return m == num;
}

static inline bool CONST is_nkind(int num, int pairs[13])
{
    int i;

    for(i = 0; i < 13; i++) {
        if(pairs[i] == num) {
            return true;
        }
    }

    return false;
}

static inline bool CONST is_fullhouse(int pairs[13]) 
{
    return is_npair(1, pairs) && is_nkind(3, pairs);
}

static inline bool CONST is_flush(int types[4])
{
    int i;
    for(i = 0; i < 5; i++) {
        if(types[i] == 5) return true;
    }
    return false;
}

static void cread(const char *line, int *p1, int *p2)
{
    char  p1i[5], p2i[5];
    char p1c[5], p2c[5];
    int i;

    sscanf(line, 
            "%c%c %c%c %c%c %c%c %c%c "
            "%c%c %c%c %c%c %c%c %c%c", 
            &p1i[0], &p1c[0],
            &p1i[1], &p1c[1],
            &p1i[2], &p1c[2],
            &p1i[3], &p1c[3],
            &p1i[4], &p1c[4],
            &p2i[0], &p2c[0],
            &p2i[1], &p2c[1],
            &p2i[2], &p2c[2],
            &p2i[3], &p2c[3],
            &p2i[4], &p2c[4]);

    for(i = 0; i < 5; i++) {
        p1[i] = decode(p1i[i],p1c[i]);
        p2[i] = decode(p2i[i],p2c[i]);
    }

    qsort(p1, 5, sizeof(int), compare_int);
    qsort(p2, 5, sizeof(int), compare_int);
}

static inline int CONST check_hand(bool hand1, bool hand2, int pairs[2][13], int nums[2][5])
{
    if(hand1 && hand2) {
        return high_card(nums[0],pairs[0],nums[1],pairs[1]) ? 1 : -1;
    } else if (!hand1 && !hand2) {
        return 0;
    } else if(hand1) {
        return 1;
    }

    return -1;
}

static int check(const char *line)
{
    int p[2][5];
    int nums[2][5] = {};
    int types[2][5] = {};
    int pairs[2][13] = {};

    cread(line,p[0],p[1]);

    getpairs(p[0],pairs[0]);
    getpairs(p[1],pairs[1]);
    getnums(p[0],nums[0]);
    getnums(p[1],nums[1]);
    gettypes(p[0],types[0]);
    gettypes(p[1],types[1]);

    int ret;
    bool straight[2];
    bool flush[2];
    bool straight_flush[2];

    straight[0] = is_straight(nums[0]);
    straight[1] = is_straight(nums[1]);
    flush[0] = is_flush(types[0]);
    flush[1] = is_flush(types[1]);
    straight_flush[0] = straight[0] && flush[0];
    straight_flush[1] = straight[1] && flush[1];

    ret = check_hand(
                straight_flush[0] && nums[0][0] == 8,
                straight_flush[1] && nums[1][0] == 8,
                pairs, nums);
    if(ret) {  return ret == 1; }

    ret = check_hand(
                straight_flush[0],
                straight_flush[1], 
                pairs, nums);
    if(ret) {  return ret == 1; }

    ret = check_hand(
                is_nkind(4, pairs[0]),
                is_nkind(4, pairs[1]),
                pairs, nums);
    if(ret) {  return ret == 1; }

    ret = check_hand(
                is_fullhouse(pairs[0]),
                is_fullhouse(pairs[1]),
                pairs, nums);
    if(ret) {  return ret == 1; }
    
    ret = check_hand(
                flush[0],
                flush[1],
                pairs, nums);
    if(ret) {  return ret == 1; }
    
    ret = check_hand(
                straight[0],
                straight[1],
                pairs, nums);
    if(ret) {  return ret == 1; }

    ret = check_hand(
                is_nkind(3, pairs[0]),
                is_nkind(3, pairs[1]),
                pairs, nums);
    if(ret) {  return ret == 1; }
    
    ret = check_hand(
                is_npair(2, pairs[0]),
                is_npair(2, pairs[1]),
                pairs, nums);
    if(ret) {  return ret == 1; }
    
    ret = check_hand(
                is_npair(1, pairs[0]),
                is_npair(1, pairs[1]),
                pairs, nums);
    if(ret) {  return ret == 1; }
    
    ret = check_hand(
                true, 
                true,
                pairs, nums);
    if(ret) {  return ret == 1; }

    ASSERT(!"BUG", abort());
}

int main(int argc, char **argv)
{
    for (int c; (c = getopt(argc, argv, "n:x:")) != -1;) {
        switch(c) {
            case 'n':
                exit(0);
            case 'x':
                DUMPD(check(optarg));
                exit(0);
        }
    }

    int result = 0;

    FILE *fp = FOPEN("r","poker.txt");
    char buf[128];

    while(fgets(buf,128,fp)) {
        result += check(buf);
    }

    FCLOSE(fp);
    DUMPD(result);

    return 0;
}
