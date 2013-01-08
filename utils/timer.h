#ifndef _COMMON_TIMER_H
#define _COMMON_TIMER_H
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <sys/time.h>
#include <stddef.h>

#include "common.h"

typedef struct timekeeper_object {
    struct timeval tv1;
    struct timeval tv2;
	long     sum;
	long     max;
	long     min;
    double   tick;
	unsigned int cnt;
} timekeeper_object;


typedef timekeeper_object *timekeeper_handle;

/******************************************************************************
 *                          INLINE FUNCTIONS
 ******************************************************************************/

static inline void timekeeper_delete(timekeeper_handle h)
{
    FREE(h);
}

static inline void timekeeper_print(timekeeper_handle h,const char *reporter)
{
    double avg = 0;
    
    ASSERT(h,ERR("timekeeper_handle is null\n"));
   
    avg = (double)h->sum / (double)h->cnt / 1000.0;
	printf("%s spends: min[%ldus], max[%ldus], avg[%.3fms], in %u times\n", reporter,h->min, h->max, avg,h->cnt);
}

static inline timekeeper_handle timekeeper_create(void)
{
    timekeeper_handle nh;
    TALLOC(nh,HDIE_NULL_D);
	nh->sum = 0;
	nh->max = 0;
	nh->min = 0x7fffffff;
	nh->cnt = 0;
    return nh;
}

static inline int timekeeper_countdown_start(timekeeper_handle h, unsigned int time)
{
    DASSERT(h, HDIE_FAILURE_D);

    //clock_gettime(CLOCK_MONOTONIC, &h->tv1);
    gettimeofday(&h->tv1, NULL);

    h->tick = (time * 1e6);

    return SUCCESS;
}

static inline int timekeeper_countdown_check_finish(timekeeper_handle h)
{
    struct timeval tv;
    double diff;

    //clock_gettime(CLOCK_MONOTONIC, &tv);
    gettimeofday(&tv, NULL);
    diff = (tv.tv_sec - h->tv1.tv_sec)  * 1e6 + (tv.tv_usec - h->tv1.tv_usec);
    //DBG("%4.2f sec to stop\n", (diff - h->tick) / 1e9);

    return diff > h->tick;
}

static inline void timekeeper_start(timekeeper_handle h)
{
    ASSERT(h,ERR("timekeeper_handle is null\n"));

    //clock_gettime(CLOCK_MONOTONIC, &h->tv1);
    gettimeofday(&h->tv1,NULL);
}


static inline void
__timekeeper_timesub(timekeeper_handle h)
{
	long            sec;
	long            usec;

	long            avg = 0;

	sec = h->tv2.tv_sec - h->tv1.tv_sec;
	usec = h->tv2.tv_usec - h->tv1.tv_usec;

	if (usec < 0) {
		sec--;
		usec += 1000000;
	}

	if (h->max < usec)
		h->max = usec;
	if (h->min > usec)
		h->min = usec;
	h->sum += usec;
	h->cnt++;
	avg = h->sum / h->cnt;
}

#define time_division(val, tv) ({ ((double) (val) / ((tv).tv_sec * 1e6 + (tv).tv_usec)) * 1e6;})

static inline void time_subtraction(struct timeval *tgt, struct timeval *lhs, struct timeval *rhs)
{
    long long           usec_diff;
    long                sec_diff;
    usec_diff = lhs->tv_usec - rhs->tv_usec;
    sec_diff = lhs->tv_sec - rhs->tv_sec;
    
    tgt->tv_sec = sec_diff;

    if(usec_diff < 0) {
        usec_diff += 1e6;
        tgt->tv_sec -= 1;
    }

    tgt->tv_usec = usec_diff;
}

static inline void timekeeper_stop(timekeeper_handle h)
{
    //clock_gettime(CLOCK_MONOTONIC, &h->tv2);
    gettimeofday(&h->tv2,NULL);

    __timekeeper_timesub(h);
}
#endif
