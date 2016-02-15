#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdio>
#include "signal.h"

//-------------------------------------------------------------------
int main(int argc, char *argv[]) {
    FILE * f = fopen ("pid_parent","w");
    fprintf(f, "%d", getpid());
    fclose(f);

    pid_t ch_pid = fork();

    if(ch_pid != 0) {
        f = fopen ("pid_child","w");
        fprintf(f, "%d", ch_pid);
        fclose(f);

        while(1) {
            int st;
            wait( &st );
        };
    }
    while(1);
    return 0;
}
//-------------------------------------------------------------------
