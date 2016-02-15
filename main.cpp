#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>

#define PID_FILE_NAME "/media/sf_mint/projects/qt/lessons/01/main.pid"
//-------------------------------------------------------------------
void * thread_func(void *args) {
    std::cout << "*** thread begin message" << std::endl;
    sleep(15);
    std::cout << "*** thread end message" << std::endl;
    return 0;
};
//-------------------------------------------------------------------
int main(int args, char ** argv) {

    pthread_t thread_id;
    pthread_attr_t attr;
    if( pthread_attr_init(&attr) != 0 ) {
        perror("POSIX error: pthread_attr_init :");
        return 0;
    }
    if( pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE) != 0) {
        perror("POSIX error: pthread_attr_setdetachstate :");
        return 0;
    }

    if( pthread_create(&thread_id, &attr, &thread_func, NULL) != 0) {
        perror("POSIX error: pthread_create :");
        return 0;
    }
    std::cout << "pthread has been created. thread_id :" << thread_id << std::endl;

    if( pthread_attr_destroy(&attr) != 0) {
        perror("POSIX error: pthread_attr_destroy :");
        return 0;
    }

    pid_t pid = getpid();
    int fd;
    if( (fd = open(PID_FILE_NAME, O_RDWR|O_CREAT, 0660))==-1) {
        perror("open error:");
        return 0;
    }
    if( write (fd, &pid, sizeof(pid_t)) == -1) {
        perror("write error:");
        return 0;
    }
    std::cout << "PID :" << pid << std::endl;

    std::cout << "waiting for joining" << std::endl;
    int status;
    if(pthread_join(thread_id, (void **)&status) != 0) {
        perror("POSIX error: pthread_join :");
        return 0;
    }
    std::cout << "Ok. pthread returns :" << status << std::endl;

    return 0;
}
//-------------------------------------------------------------------
