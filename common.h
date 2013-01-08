#ifndef _COMMON_COMMON_H
#define _COMMON_COMMON_H

#include <unistd.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>

#define likely(x)   __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)

#define ICHECK do {\
    INFO("(*^-^)v\n");} while (0)

#define WARN(fmt, args...) do { \
		fprintf(stderr, "\x1b[35mWARN :%s:%s:%d: " fmt "\x1b[0m",  __BASE_FILE__, __FUNCTION__, __LINE__, ## args); \
	} while (0)

#define INFO(fmt, args...) do { \
		fprintf(stderr, "\x1b[36mINFO :%s:%s:%d: " fmt "\x1b[0m",  __BASE_FILE__, __FUNCTION__, __LINE__, ## args); \
	} while (0)

#define ERR(fmt, args...) do { \
		fprintf(stderr, "\x1b[1;31mERR  :%s:%s:%d: " fmt "\x1b[0m",  __BASE_FILE__, __FUNCTION__, __LINE__, ## args); \
	} while (0)

#define DUMPD(x) printf("%s = %d\n", #x, (int) (x))
#define DUMPL(x) printf("%s = %llu\n", #x, (uint64_t) (x))
#define DUMPS(x) printf("%s = %s\n", #x, (char *) (x))

#define MASSERT(b,action,fmt,args...) do {if(unlikely(!(b))){ ERR("failed (%s): " fmt "\n", #b, ##args);action;} } while (0)

#ifdef __DEBUG
#define DD(x) printf(" %d ", (int) (x))

#define DBG( fmt, args...) do { \
		fprintf(stderr, "DBG  :%s:%s:%d: " fmt,  __BASE_FILE__, __FUNCTION__, __LINE__, ## args); \
	} while (0)


#define CHECK do {\
    DBG("(*^-^)b\n");} while (0)

#define DASSERT(b,action) do {if(unlikely(!(b))){ ERR("debug assertion failure (%s)\n", #b);action;} } while (0)

#define MUST(b,action) do {if(unlikely(!(b))){ ERR("assertion failure (%s)\n", #b);action;} } while (0)

#ifdef __VERBOSE
#define ASSERT(b,action) do {CHECK;if(unlikely(!(b))){ ERR("failed (%s)\n", #b);action;} CHECK;} while (0)
#else
#define ASSERT(b,action) do {if(unlikely(!(b))){ ERR("failed (%s)\n", #b);action;}} while (0)
#endif

#define EXPECT(b,action) do {if(unlikely(!(b))){ ERR("unexpected (%s)\n", #b);action;} } while (0)

#else
#define DD(x) 

#define DBG( fmt, args...)

#define CHECK 

#define DASSERT(b,action) do {if(unlikely(!(b))){ ERR("debug assertion failure (%s)\n", #b);action;} } while (0)

#define MUST(b,action) do {if(unlikely(!(b))){ ERR("assertion failure (%s)\n", #b);action;} } while (0)

#define ASSERT(b,action) do {if(unlikely(!(b))){ ERR("assertion failure (%s)\n", #b);action;} } while (0)
#define EXPECT(b,action) do {if(unlikely(!(b))){ ERR("unexpected (%s)\n", #b);action;} } while (0)

#endif

#define HDIE_FAILURE_D do {return FAILURE;} while (0) 
#define HDIE_FAILURE(fmt,args...) do { ERR(fmt, ## args);HDIE_FAILURE_D;} while (0)
#define HDIE_NULL_D do {return NULL;} while (0) 
#define HDIE_NULL(fmt,args...) do { ERR(fmt, ## args);HDIE_NULL_D;} while (0)
#define HGO_ERROR_D do {goto error;} while (0) 
#define HGO_ERROR(fmt,args...) do { ERR(fmt, ## args);HGO_ERROR_D;} while (0)
        /* common object creation macro: calloc given pointer. when failed, perform 'action' */
#define SUCCESS         (0)
#define FAILURE         (-1)

#define CLEAR(x) memset (&(x), 0, sizeof (x))
#define CLOSE(x) do{if((x)>0){close(x);(x) = 0;}}while(0)
#define FCLOSE(x) do{if((x)!=NULL){fclose(x);(x) = NULL;}}while(0)
#define ALLOC(t) (t = (typeof(t))calloc(1,sizeof(typeof(*(t)))))

#define REALLOC(t,num,action) ({ typeof(t) ___realloc_tmp; ASSERT(___realloc_tmp = (typeof(t))realloc((t),(num) * sizeof(typeof(*(t)))), action); (t) = ___realloc_tmp;})

#define NEW(type, ...) ({type __x = {__VA_ARGS__};type *__p = malloc(sizeof(type));__p ? (memcpy(__p,&__x,sizeof(type)), __p) : NULL;}) 

#define COPY(x,y) memcpy ((x),(y),sizeof (*(y)))
#define FREE(h) do { if(h){free(h);(h)=NULL;}} while (0);

#define TALLOC(h,action) do {h=NULL;if(!(ALLOC(h))){ERR("No memory!\n");action;}} while (0)
#define TALLOCS(tgt,num,action) ({ if(!((tgt) = (typeof (tgt))(calloc((sizeof (typeof(*(tgt)))), num)))){ERR("No memory!\n");action;} })
#define CLEAR_ARRAY(a,sz) ({memset(a,0,sizeof(*(a)) * (sz));})
#define SATURATE(x) ({(x) = min(1.0f,x);})
    
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef min
#define min(a,b) ((a)>(b)?(b):(a))
#endif

#ifndef range
#define range(x,l,r) (min(max(l,x),r))
#endif

#ifndef swap
#define swap(l,r) ({typeof(l) ____tmp = l; l = r; r = ____tmp;})
#endif

#define TRUE            1
#define FALSE           0

#define CHECK_MAGIC(magic,ptr) (*(ptr) && (((unsigned int*)(*(ptr)))[0] == (magic)))
 
#define SELECT(fd,sec) ({ \
        fd_set rfds; \
        struct timeval timeout = {.tv_sec = (sec)}; \
        FD_ZERO(&rfds); \
        FD_SET(fd, &rfds); \
        select((fd) + 1,&rfds,NULL,NULL,&timeout);})

#ifndef container_of
#define container_of(type,ptr,member) ({ const typeof( ((type *)0)->member) *__mptr=(ptr); (type *) ( (char *)__mptr - offsetof(type,member));})
#endif

#define PURE __attribute__((pure))
#define PACKED __attribute__((packed))
#define CONST __attribute__((const))
#define UNUSED __attribute__((unused))
#define DEPRECATED __attribute__((deprecated))
#define CONSTRUCTOR __attribute__((constructor))
#define DESTRUCTOR __attribute__((destructor))

static inline void
dump8(char *p,size_t len)
{
    int i;
    unsigned int v;

    for(i = 0; i<len; i += 8) {
        v = p[i];
        printf("%02x ",v);
        v = p[i+1];
        printf("%02x ",v);
        v = p[i+2];
        printf("%02x ",v);
        v = p[i+3];
        printf("%02x|",v);
        v = p[i+4];
        printf("%02x ",v);
        v = p[i+5];
        printf("%02x ",v);
        v = p[i+6];
        printf("%02x ",v);
        v = p[i+7];
        printf("%02x\n",v);
    }
}


static inline void
dump32(int *p,size_t len)
{
    int i;
    unsigned int v;

    for(i = 0; i<len; i += 8) {
        v = p[i];
        printf("%04x ",v);
        v = p[i+1];
        printf("%04x ",v);
        v = p[i+2];
        printf("%04x ",v);
        v = p[i+3];
        printf("%04x|",v);
        v = p[i+4];
        printf("%04x ",v);
        v = p[i+5];
        printf("%04x ",v);
        v = p[i+6];
        printf("%04x ",v);
        v = p[i+7];
        printf("%04x\n",v);
    }
}


static inline void
dumpf(float *p,size_t len)
{
    int i;
    float v;

    for(i = 0; i<len; i ++) {
        v = p[i];
        if(i % 10 == 0) {
            printf("(%d/%zd)\t",i,len);
        }
        printf("%.3f",v);

        if(i > 0 && (i%10) == 9) {
            printf("\n");
        } else {
            printf("\t");
        }
    }
    printf("\n");
}
static inline void
dumpy(char *p,size_t len)
{
    int i;
    unsigned int v;

    for(i = 0; i<len; i += 16) {
        v = p[i];
        printf("%02x ",v);
        v = p[i+2];
        printf("%02x ",v);
        v = p[i+4];
        printf("%02x ",v);
        v = p[i+6];
        printf("%02x|",v);
        v = p[i+8];
        printf("%02x ",v);
        v = p[i+10];
        printf("%02x ",v);
        v = p[i+12];
        printf("%02x ",v);
        v = p[i+14];
        printf("%02x\n",v);
    }
}
#endif				
