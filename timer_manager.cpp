
#include "timer_util.h"

#include <map>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>

#include <sys/un.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/timerfd.h>

#define MAX 10

using namespace std;

static fd_set g_set;

int listen(int* data);

int main(int argc, char* argv[]){
	struct itimerspec it = get_interval_itimerspec(3, 0);
	int fds[MAX];
	for(int i=0; i<MAX; i++){
		fds[i] = util_create_timer(&it);
		cout<<"fd = "<<fds[i]<<endl;
	}
	listen(fds);
}

int listen(int* data) {

    int ret = -1;

    int maxfd = data[MAX-1];
    int tmpfd = -1;
    long tmpdata = 0;

    fd_set set;

    FD_ZERO(&g_set);
	for(int i=0; i<MAX; i++){
		FD_SET(data[i], &g_set);
	}

    while(true) {

        set = g_set;
        ret = select(maxfd+1, &set, NULL, NULL, NULL);

        switch(ret){

        case -1:
            cout<<"select return -1."<<endl;
            break;
        case 0:
            cout<<"select return 0."<<endl;
            break;
        default:
            for (tmpfd=0; tmpfd<(maxfd+1); tmpfd++){
                ret = FD_ISSET(tmpfd, &set);
                do {
                    ret = read(tmpfd, (char *)&tmpdata, sizeof(long));
					if (ret == 0 ){
					}else{
						cout<<"tmpfd = "<<tmpfd<<endl;
					}
                    switch(ret)
                    {
                    case EAGAIN:
                        cout<<"timerfd read EAGAIN."<<endl;
                        break;
                    case -1:
                        break;
                    case 0:
                        break;
                    default:
                        //FD_CLR(tmpfd,&g_set);
                        cout<<"fd:data -> "<<tmpfd<<":"<<tmpdata<<endl;
						//struct itimerspec new_v = get_once_itimerspec(3, 0);
						//util_update_timer(tmpfd, &new_v);
                        break;
                    }
                }while(ret>0);
            }
            break;
        }
    }
    return 0;
}
