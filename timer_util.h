
#ifndef __TIMER_UTIL_H__
#define __TIMER_UTIL_H__

typedef enum TIMER_RESULT{
	TIMER_RESULT_SUCCESS = 0,
	TIMER_RESULT_FAILED = 1,
}TIMER_RESULT;

int util_create_timer(struct itimerspec* new_value);
TIMER_RESULT util_update_timer(int fd, struct itimerspec* new_value);
TIMER_RESULT util_delete_timer(int fd);
struct itimerspec get_new_itimerspec_from_now(int sec, int nsec);

#endif //__TIMER_UTIL_H__

