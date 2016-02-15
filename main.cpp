#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include "signal.h"

//-------------------------------------------------------------------
int main(int argc, char *argv[])
{
    FILE * p_file = fopen ("pid","w");
    fprintf(p_file, "%d", getpid());
    fclose(p_file);

    signal(SIGTERM, SIG_IGN);
    signal(SIGINT, SIG_IGN);

    while(1);

    return 0;
}
//-------------------------------------------------------------------
