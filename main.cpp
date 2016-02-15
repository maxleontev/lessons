#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <stdlib.h>
#include <stdio.h>

//-------------------------------------------------------------------
using namespace std;
//-------------------------------------------------------------------
int main(int argc, char *argv[]) {

    int sockets[2];
    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sockets) < 0) {
        cout << "socketpair() failed" << endl;
        return 0;
    }

    if (fork() != 0)
        close(sockets[1]);
    else
        close(sockets[0]);

    while(1);

    return 0;
}
//-------------------------------------------------------------------
