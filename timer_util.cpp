
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <sys/timerfd.h>
#include "timer_util.h"


/*  function: util_create_timer
 *	description: create timer and bind to nonblocking fd.
 **/
int util_create_timer(struct itimerspec* new_v){
	int fd = -1;
	fd = timerfd_create(CLOCK_REALTIME, TFD_NONBLOCK);
    if (fd == -1){
		return fd;
    }
    int result = timerfd_settime(fd, TFD_TIMER_ABSTIME, new_v, NULL);
    if (result == -1){
		close(fd);
		fd = -1;
		return fd;
    }
	return fd;
}


/*  function: util_update_timer
 *	description: update timer.
 **/
TIMER_RESULT util_update_timer(int fd, struct itimerspec* new_v){
    struct itimerspec old_v;
    int result = timerfd_gettime(fd, &old_v);
    if (result == -1){
		return TIMER_RESULT_FAILED;
    }
    result = timerfd_settime(fd, TFD_TIMER_ABSTIME, new_v, &old_v);
    if (result == -1){
		return TIMER_RESULT_FAILED;
    }
	return TIMER_RESULT_SUCCESS;
}


/*  function: util_delete_timer
 *  description: delete timer.
 **/
TIMER_RESULT util_delete_timer(int& fd){
	int result = close(fd);
	if (result < 0){
		return TIMER_RESULT_FAILED;
	}
	fd = -1;
	return TIMER_RESULT_SUCCESS;
}


/*  function: get_once_itimerspec
 *  description:
 *  note: nsec is not over 1000000000
 * */
struct itimerspec get_once_itimerspec(int sec, int nsec){
	struct timespec now;
	struct itimerspec it;
	clock_gettime(CLOCK_REALTIME, &now);
	it.it_value.tv_sec = now.tv_sec + sec;
	it.it_value.tv_nsec = now.tv_nsec + nsec; 
	it.it_interval.tv_sec = 0;
	it.it_interval.tv_nsec = 0;
}


/*  function: get_interval_itimerspec
 *  description:
 *  note: nsec is not over 1000000000
 * */
struct itimerspec get_interval_itimerspec(int sec, int nsec){
	struct timespec now;
	struct itimerspec it;
	clock_gettime(CLOCK_REALTIME, &now);
	it.it_value.tv_sec = now.tv_sec + sec;
	it.it_value.tv_nsec = now.tv_sec + nsec; 
	it.it_interval.tv_sec = sec;
	it.it_interval.tv_nsec = nsec;
}

