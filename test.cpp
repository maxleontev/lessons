#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


//-------------------------------------------------------------------
#define SH_MEM_SIZE 1024 * 1024 * 2
//-------------------------------------------------------------------
int main() {
    key_t key;
    if ( (key = ftok("/tmp/mem.temp", 1)) == (key_t) -1) {
        perror("SysV error: ftok :");
        return 0;
    }
    printf("key : %d\r\n", key);

    int shm_id;
    if( (shm_id = shmget(key, SH_MEM_SIZE, 0666)) == -1) {
        perror("SysV error: shmget :");
        return 0;
    }

    printf("shm_id : %d\r\n", shm_id);

    char * shm;
    if( (shm = (char *) shmat(shm_id, NULL, 0)) == (char *) -1) {
          perror("SysV error: shmat :");
          return 0;
    }

    printf("%d %d %d %d \r\n", *(shm+0), *(shm+1), *(shm+2), *(shm+3) );

    return 0;
}
//-------------------------------------------------------------------
