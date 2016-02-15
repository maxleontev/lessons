#include <iostream>
#include <stdio.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

//-------------------------------------------------------------------
int main(int args, char ** argv) {

    sem_t * key;
    if( (key = sem_open("/test.sem", O_CREAT, 0666, 66)) == SEM_FAILED ) {
        perror("POSIX error: sem_open :");
        return 0;
    }

    // next code is not for examination task, it's for me only
    int t;
    std::cout << "for close and unlink type '1'" << std::endl;
    std::cin >> t;
    if(t == 1) {
        if( sem_close(key) == -1) {
            perror("POSIX error: sem_close :");
            return 0;
        }
        if( sem_unlink("/test.sem") == -1 ) {
            perror("POSIX error: sem_unlink :");
            return 0;
        }
        std::cout << "close and unlink ok" << std::endl;
    }

    return 0;
}
//-------------------------------------------------------------------
