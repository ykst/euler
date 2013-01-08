#ifndef _VIDCAP_FILEOP_H
#define _VIDCAP_FILEOP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include "common.h"

/******************************************************************************
 *                  MACROS
 ******************************************************************************/
#define FOPEN(attr,fmt, args...) ({ \
    FILE *_fp = NULL; \
    char *_fpath = malloc(FILENAME_MAX); \
    if (_fpath) { \
        snprintf(_fpath, FILENAME_MAX, fmt, ##args);\
        _fp = fopen(_fpath, attr);\
        free(_fpath);\
    }\
    _fp;})

#define FREMOVE(fmt, args...) ({ \
    int _ret = FAILURE; \
    char *_fpath = malloc(FILENAME_MAX); \
    if (_fpath) { \
        snprintf(_fpath, FILENAME_MAX, fmt, ##args);\
        _ret = remove(_fpath);\
        free(_fpath);\
    }\
    _ret;})

/* file size... yeah, this is not portable indeed */
#define FSIZE(fp) ({ \
    size_t _ret = 0; \
    fseek(fp, 0, SEEK_END);\
    _ret = ftell(fp); \
    rewind(fp); \
    _ret;})

#define FCLOSE(x) do{if((x)!=NULL){fclose(x);(x) = NULL;}}while(0)

/******************************************************************************
 *                  PUBLIC FUNCTIONS
 ******************************************************************************/
static inline int remove_dir_files(const char *dir);
static inline int iterate_dir_files(const char *dir, int (*func)(const char *, void *), void *v);

static inline int iterate_dir_files(const char *dir, int (*func)(const char *, void *), void *v)
{
    DASSERT(func, return FAILURE);
    DASSERT(dir, return FAILURE);
    DASSERT(strlen(dir) > 1, HDIE_FAILURE_D);

    DIR *dp = NULL;
    int ret = FAILURE;
    int cnt = 0;
    struct dirent e = {};
    struct dirent *ep = NULL;
    char fpath[FILENAME_MAX];

    ASSERT(dp = opendir(dir), HDIE_FAILURE_D);
    while(readdir_r(dp,&e,&ep) == 0) {
        if(!ep) break;
        if(ep->d_type == DT_REG) {
            snprintf(fpath,FILENAME_MAX,"%s/%s",dir, ep->d_name);
            ASSERT(func(fpath,v) == SUCCESS, goto error);
        }
#ifdef __DEBUG
        if(cnt%1000 ==  999) {
            printf("cnt = %d\n",cnt);
        }
#endif
    }

    ret = SUCCESS;
error:
    if(dp) closedir(dp);

    return ret == SUCCESS ? cnt : -1;
}

static inline int __remove_entry(const char *fpath, void *v)
{
    ASSERT(remove(fpath) == 0, return FAILURE);

    return SUCCESS;
}


static inline int remove_dir_files(const char *dir)
{
    return iterate_dir_files(dir, __remove_entry, NULL);
}

#endif
