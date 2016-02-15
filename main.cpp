#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

//-------------------------------------------------------------------
#define SH_MEM_SIZE 1024 * 1024 * 2
//-------------------------------------------------------------------
int main(int args, char ** argv) {

    key_t key;
    if ( (key = ftok("/tmp/mem.temp", 1)) == (key_t) -1) {
        perror("SysV error: ftok :");
        return 0;
    }
    printf("key : %d\r\n", key);

    int shm_id;
    if( (shm_id = shmget(key, SH_MEM_SIZE, IPC_CREAT | 0666)) == -1) {
        perror("SysV error: shmget :");
        return 0;
    }
    printf("shm_id : %d\r\n", shm_id);

    char * shm;
    if( (shm = (char *) shmat(shm_id, NULL, 0)) == (char *) -1) {
          perror("SysV error: shmat :");
          return 0;
    }

    for (int i = 0; i < 1024 * 1024; i++)
            *(shm + i) = i%127;

    printf("press 1 to detach shared memory and exit\r\n");
    int d = 0;
    std::cin >> d;
    if(d==1) {
        if( shmdt(shm) == -1) {
            perror("SysV error: shmdt :");
            return 0;
        }
    }

    return 0;
}
//-------------------------------------------------------------------
