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
    pthread_mutex_t* m = (pthread_mutex_t*) arg;

    if( pthread_mutex_lock(m) != 0) {
        perror("* POSIX error: pthread_mutex_lock :");
        return 0;
    }
    std::cout << "* thread 1 mutex has been locked" << std::endl;

    sleep(5);

    if( pthread_mutex_unlock(m) != 0) {
        perror("* POSIX error: pthread_mutex_unlock :");
        return 0;
    }
    std::cout << "* thread 1 mutex has been unlocked" << std::endl;

    std::cout << "* thread 1 end message" << std::endl;
    return 0;
};
//-------------------------------------------------------------------
void * thread_2_func(void *arg) {
    std::cout << "** thread 2 begin message" << std::endl;
    pthread_spinlock_t * spinlock = (pthread_spinlock_t *) arg;

    if( pthread_spin_lock(spinlock) != 0 ) {
        perror("** POSIX error: pthread_spin_lock:");
        return 0;
    }
    std::cout << "** thread 2 spin has been locked" << std::endl;

    sleep(6);

    if( pthread_spin_unlock(spinlock) != 0 ) {
        perror("** POSIX error: pthread_spin_unlock:");
        return 0;
    }
    std::cout << "** thread 2 spin has been unlocked" << std::endl;

    std::cout << "** thread 2 end message" << std::endl;
    return 0;
}
//-------------------------------------------------------------------
void * thread_3_func(void *arg) {
    std::cout << "*** thread 3 begin message" << std::endl;
    pthread_rwlock_t * wr_lock = (pthread_rwlock_t *) arg;

    if( pthread_rwlock_wrlock(wr_lock) != 0) {
        perror("*** POSIX error: pthread_rwlock_wrlock :");
        return 0;
    }
    std::cout << "*** thread 3 wr_lock has been write locked" << std::endl;

    sleep(7);

    if( pthread_rwlock_unlock(wr_lock) !=0 ) {
        perror("*** POSIX error: pthread_rwlock_unlock :");
        return 0;
    }
    std::cout << "*** thread 3 wr_lock has been unlocked" << std::endl;

    std::cout << "*** thread 3 end message" << std::endl;
    return 0;
}
//-------------------------------------------------------------------
void * thread_4_func(void *arg) {
    std::cout << "**** thread 4 begin message" << std::endl;
    pthread_rwlock_t * rd_lock = (pthread_rwlock_t *) arg;

    if( pthread_rwlock_rdlock(rd_lock) != 0) {
        perror("**** POSIX error: pthread_rwlock_rdlock :");
        return 0;
    }
    std::cout << "**** thread 4 rd_lock has been read locked" << std::endl;

    sleep(8);

    if( pthread_rwlock_unlock(rd_lock) !=0 ) {
        perror("**** POSIX error: pthread_rwlock_unlock :");
        return 0;
    }
    std::cout << "**** thread 4 rd_lock has been unlocked" << std::endl;

    std::cout << "**** thread 4 end message" << std::endl;
    return 0;
}
//-------------------------------------------------------------------
int main(int args, char ** argv) {
    if( save_pid() == 0)
        return 0;

    std::cout << "---------------------------------------" << std::endl;
    // // // thread 1 // // // // // //
    pthread_t thread_1_id;
    pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
    if(pthread_mutex_lock(&mut) != 0) {
        perror("POSIX error: pthread_mutex_lock :");
        return 0;
    }
    std::cout << "mutex has been locked" << std::endl;

    if( pthread_create(&thread_1_id, NULL, &thread_1_func, (void *) &mut) != 0) {
        perror("POSIX error: pthread_create 1:");
        return 0;
    }
    std::cout << "pthread has been created. thread_1_id :" << thread_1_id << std::endl;
    std::cout << "---------------------------------------" << std::endl;

    // // // thread 2 // // // // // //
    pthread_t thread_2_id;
    pthread_spinlock_t spinlock;

    if( pthread_spin_init(&spinlock, PTHREAD_PROCESS_SHARED) != 0) {
        perror("POSIX error: pthread_spin_init :");
        return 0;
    }

    if( pthread_spin_lock(&spinlock) != 0 ) {
        perror("POSIX error: pthread_spin_lock:");
        return 0;
    }
    std::cout << "spin has been locked" << std::endl;

    if( pthread_create(&thread_2_id, NULL, &thread_2_func, (void *) &spinlock) != 0) {
        perror("POSIX error: pthread_create 2:");
        return 0;
    }
    std::cout << "pthread has been created. thread_2_id :" << thread_2_id << std::endl;
    std::cout << "---------------------------------------" << std::endl;

    // // // thread 3 // // // // // //
    pthread_t thread_3_id;
    pthread_rwlock_t wr_lock = PTHREAD_RWLOCK_INITIALIZER;

    if( pthread_rwlock_wrlock(&wr_lock) != 0) {
        perror("POSIX error: pthread_rwlock_wrlock :");
        return 0;
    }
    std::cout << "wr_lock has been write locked" << std::endl;

    if( pthread_create(&thread_3_id, NULL, &thread_3_func, (void *) &wr_lock) != 0) {
        perror("POSIX error: pthread_create 3:");
        return 0;
    }
    std::cout << "pthread has been created. thread_3_id :" << thread_3_id << std::endl;
    std::cout << "---------------------------------------" << std::endl;

    // // // thread 4 // // // // // //
    pthread_t thread_4_id;
    pthread_rwlock_t rd_lock;
    pthread_rwlockattr_t attr;
    if( pthread_rwlockattr_init(&attr) != 0) {
        perror("POSIX error: pthread_rwlockattr_init :");
        return 0;
    }
    if( pthread_rwlockattr_setkind_np(&attr, PTHREAD_RWLOCK_PREFER_WRITER_NP) != 0 ) {
        perror("POSIX error: pthread_rwlockattr_setkind_np :");
        return 0;
    }
    if( pthread_rwlock_init(&rd_lock, &attr) != 0 ) {
        perror("POSIX error: pthread_rwlock_init :");
        return 0;
    }

    // If we call pthread_rwlock_rdlock, then thread4 not stop at
    // calling function pthread_rwlock_rdlock(&rd_lock)
    if( pthread_rwlock_wrlock(&rd_lock) != 0) {
        perror("POSIX error: pthread_rwlock_rdlock :");
        return 0;
    }
    std::cout << "rd_lock has been read locked" << std::endl;

    if( pthread_create(&thread_4_id, NULL, &thread_4_func, (void *) &rd_lock) != 0) {
        perror("POSIX error: pthread_create 4:");
        return 0;
    }
    std::cout << "pthread has been created. thread_4_id :" << thread_3_id << std::endl;
    std::cout << "---------------------------------------" << std::endl;

    sleep(15);

    std::cout << "---------------------------------------" << std::endl;
    if( pthread_mutex_unlock(&mut) != 0) {
        perror("POSIX error: pthread_mutex_unlock :");
        return 0;
    }
    std::cout << "mutex has been unlocked" << std::endl;
    int status;
    if(pthread_join(thread_1_id, (void **)&status) != 0) {
        perror("POSIX error: pthread_join 1:");
        return 0;
    }
    std::cout << "Ok. pthread 1 returns :" << status << std::endl;
    if( pthread_mutex_destroy(&mut) != 0 ) {
        perror("POSIX error: pthread_mutex_destroy:");
        return 0;
    }
    std::cout << "---------------------------------------" << std::endl;

    if( pthread_spin_unlock(&spinlock) != 0) {
        perror("POSIX error: pthread_spin_unlock :");
        return 0;
    }
    std::cout << "spin has been unlocked" << std::endl;
    if(pthread_join(thread_2_id, (void **)&status) != 0) {
        perror("POSIX error: pthread_join 2:");
        return 0;
    }
    std::cout << "Ok. pthread 2 returns :" << status << std::endl;
    if( pthread_spin_destroy(&spinlock) != 0) {
        perror("POSIX error: pthread_spin_destroy :");
        return 0;
    }
    std::cout << "---------------------------------------" << std::endl;

    if( pthread_rwlock_unlock(&wr_lock) !=0 ) {
        perror("POSIX error: pthread_rwlock_unlock :");
        return 0;
    }
    std::cout << "wr_lock has been unlocked" << std::endl;

    if(pthread_join(thread_3_id, (void **)&status) != 0) {
        perror("POSIX error: pthread_join 3:");
        return 0;
    }
    std::cout << "Ok. pthread 3 returns :" << status << std::endl;
    if( pthread_rwlock_destroy(&wr_lock) != 0 ) {
        perror("POSIX error: pthread_rwlock_destroy :");
        return 0;
    }
    std::cout << "---------------------------------------" << std::endl;

    if( pthread_rwlock_unlock(&rd_lock) !=0 ) {
        perror("POSIX error: pthread_rwlock_unlock :");
        return 0;
    }
    std::cout << "rd_lock has been unlocked" << std::endl;

    if(pthread_join(thread_4_id, (void **)&status) != 0) {
        perror("POSIX error: pthread_join 4:");
        return 0;
    }
    std::cout << "Ok. pthread 4 returns :" << status << std::endl;
    if( pthread_rwlock_destroy(&rd_lock) != 0 ) {
        perror("POSIX error: pthread_rwlock_destroy :");
        return 0;
    }
    std::cout << "---------------------------------------" << std::endl;

    return 0;
}
//-------------------------------------------------------------------
