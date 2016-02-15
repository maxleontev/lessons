#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

//-------------------------------------------------------------------
#define SH_MEM_SIZE 1024 * 1024
#define SH_MEM_NAME "/test.shm"
//-------------------------------------------------------------------
int main(int args, char ** argv) {

    int shm_key;
    if( (shm_key = shm_open(SH_MEM_NAME, O_CREAT | O_RDWR, 0666))== -1) {
        perror("POSIX error: shm_open :");
        return 0;
    }
    printf("shm_key : %d\r\n", shm_key);

    if(ftruncate(shm_key, SH_MEM_SIZE) == -1) {
        perror("POSIX error: ftruncate :");
        return 0;
    }

    char * ptr;
    if( (ptr = (char *) mmap(NULL, SH_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_key, 0)) == (char *) -1) {
        perror("POSIX error: mmap :");
        return 0;
    }

    for(int i=0; i<SH_MEM_SIZE; i++)
        *(ptr + i) = 13;

    printf("press 1 to unlink shared memory and exit\r\n");
    int d = 0;
    std::cin >> d;
    if(d==1) {
        if( munmap(ptr, SH_MEM_SIZE) == -1) {
            perror("POSIX error: munmap :");
            return 0;
        }
        if( shm_unlink(SH_MEM_NAME) == -1) {
            perror("POSIX error: shm_unlink :");
            return 0;
        }
    }

    return 0;
}
//-------------------------------------------------------------------
