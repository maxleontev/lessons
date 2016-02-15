#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <fcntl.h>

//-------------------------------------------------------------------
#define PID_FILE_NAME "/media/sf_mint/projects/qt/lessons/01/main.pid"
int save_pid() {
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
    return 1;
}
//-------------------------------------------------------------------
void * thread_1_func(void *arg) {
    std::cout << "* thread 1 begin message" << std::endl;
    pthread_cond_t * cond = (pthread_cond_t *) arg;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

    std::cout << "* thread 1 start waiting of the condition" << std::endl;
    if( pthread_cond_wait(cond, &mutex) != 0 ) {
        perror("* POSIX error: pthread_cond_wait :");
        return 0;
    }
    std::cout << "* thread 1 wait of the condition is over" << std::endl;

    sleep(5);

    std::cout << "* thread 1 end message" << std::endl;
    return 0;
};
//-------------------------------------------------------------------
void * thread_2_func(void *arg) {
    std::cout << "** thread 2 begin message" << std::endl;
    pthread_barrier_t * barrier = (pthread_barrier_t *) arg;

    std::cout << "** thread 2 barrier waiting" << std::endl;
    int w_res = pthread_barrier_wait(barrier);
    if( w_res != 0 && w_res != PTHREAD_BARRIER_SERIAL_THREAD ) {
        perror("** POSIX error: pthread_barrier_wait :");
        return 0;
    }
    std::cout << "** thread 2 barrier reached" << std::endl;

    sleep(5);

    std::cout << "** thread 2 end message" << std::endl;
    return 0;
};
//-------------------------------------------------------------------
int main(int args, char ** argv) {
    if( save_pid() == 0)
        return 0;

    std::cout << "---------------------------------------" << std::endl;
    // // // thread 1 // // // // // //
    pthread_t thread_1_id;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

    if( pthread_cond_init(&cond, NULL) != 0) {
        perror("POSIX error: pthread_cond_init:");
        return 0;
    }

    if( pthread_create(&thread_1_id, NULL, &thread_1_func, (void **) &cond ) != 0) {
        perror("POSIX error: pthread_create 1:");
        return 0;
    }
    std::cout << "pthread has been created. thread_1_id :" << thread_1_id << std::endl;
    std::cout << "---------------------------------------" << std::endl;

    // // // thread 2 // // // // // //
    pthread_t thread_2_id;
    pthread_barrier_t barrier;

    if( pthread_barrier_init(&barrier, NULL, 2) != 0 ) {
        perror("POSIX error: pthread_barrier_init:");
        return 0;
    }

    if( pthread_create(&thread_2_id, NULL, &thread_2_func, (void **) &barrier ) != 0) {
        perror("POSIX error: pthread_create 2:");
        return 0;
    }
    std::cout << "pthread has been created. thread_2_id :" << thread_2_id << std::endl;
    std::cout << "---------------------------------------" << std::endl;

    sleep(15);

    std::cout << "---------------------------------------" << std::endl;
    if( pthread_cond_signal(&cond) != 0 ) {
        perror("POSIX error: pthread_cond_signal:");
        return 0;
    }
    std::cout << "pthread cond signal has been sended" << std::endl;

    int status;
    if(pthread_join(thread_1_id, (void **)&status) != 0) {
        perror("POSIX error: pthread_join 1:");
        return 0;
    }
    std::cout << "Ok. pthread 1 returns :" << status << std::endl;

    if( pthread_cond_destroy(&cond) != 0 ) {
        perror("POSIX error: pthread_cond_destroy:");
        return 0;
    }
    std::cout << "---------------------------------------" << std::endl;

    std::cout << "pthread barrier waiting" << std::endl;

    int w_res = pthread_barrier_wait(&barrier);
    if( w_res != 0 && w_res != PTHREAD_BARRIER_SERIAL_THREAD ) {
        perror("POSIX error: pthread_barrier_wait :");
        return 0;
    }
    std::cout << "pthread barrier reached" << std::endl;

    if(pthread_join(thread_2_id, (void **)&status) != 0) {
        perror("POSIX error: pthread_join 2:");
        return 0;
    }
    std::cout << "Ok. pthread 2 returns :" << status << std::endl;

    if( pthread_barrier_destroy(&barrier) != 0 ) {
        perror("POSIX error: pthread_barrier_destroy:");
        return 0;
    }
    std::cout << "---------------------------------------" << std::endl;

    return 0;
}
//-------------------------------------------------------------------
