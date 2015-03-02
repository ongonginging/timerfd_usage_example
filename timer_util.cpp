

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
TIMER_RESULT util_delete_timer(int fd){
	int result = close(fd);
	if (result < 0){
		return TIMER_RESULT_FAILED;
	}
	return TIMER_RESULT_SUCCESS;
}

struct itimerspec get_new_itimerspec_from_now(int sec, int nsec){
	struct timespec now;
	struct itimerspec new_value;
	clock_gettime(CLOCK_REALTIME, &now);
	new_value.it_value.tv_sec = now.tv_sec + sec;
	new_value.it_value.tv_nsec = now.tv_nsec + nsec; 
	new_value.it_interval.tv_sec = 0;
	new_value.it_interval.tv_nsec = 0;
	return new_value;
}
