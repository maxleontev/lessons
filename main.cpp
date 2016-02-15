#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

//-------------------------------------------------------------------
int main(int args, char ** argv) {

    key_t key;
    if ( (key = ftok("/tmp/sem.temp", 1)) == (key_t) -1) {
        perror("SysV error: ftok :");
        return 0;
    }

    int sem_id;
    if( (sem_id = semget(key, 16, 0666 | IPC_CREAT)) == -1 ) {
        perror("SysV error: semget :");
        return 0;
    }

    struct sembuf opts[16];
    for (int i = 0; i < 16; i++) {
        opts[i].sem_num = i;
        opts[i].sem_op = i;
        opts[i].sem_flg = 0;
    }
    if( semop(sem_id, opts, 16) == -1 ) {
        perror("SysV error: semop :");
        return 0;
    }

    return 0;
}
//-------------------------------------------------------------------
