/*
#include <sstream>
#include <vector>
#include <fcntl.h>
#include <fstream>
#include <unistd.h>
*/

#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <unistd.h>

#include <fcntl.h>

//-------------------------------------------------------------------

/*
#define IN_F_N "/home/box/in.fifo"
#define OUT_F_N "/home/box/out.fifo"*/

#define IN_F_N "/tmp/in.fifo"
#define OUT_F_N "/tmp/out.fifo"

/*#define IN_F_N "/media/sf_mint/projects/qt/lessons/01/in.fifo"
#define OUT_F_N "/media/sf_mint/projects/qt/lessons/01/out.fifo"*/


using namespace std;
//-------------------------------------------------------------------
int main(int argc, char *argv[]) {

    unlink(IN_F_N);
    unlink(OUT_F_N);

    if( (mkfifo(IN_F_N, 0660)) == -1) {
        cout << "error creating in_fifo" << endl;
        return 0;
    }
    int in_fifo;
    if( (in_fifo = open(IN_F_N, O_RDWR)) == -1 ) {
        cout << "error open in_fifo" << endl;
        return 0;
    }

    if( (mkfifo(OUT_F_N, 0660)) == -1) {
        cout << "error creating out_fifo" << endl;
        return 0;
    }
    int out_fifo;
    if( (out_fifo = open(OUT_F_N, O_RDWR)) == -1 ) {
        cout << "error open out_fifo" << endl;
        return 0;
    }

    int ln;
    char buf[1];
    while ((ln = read(in_fifo, buf, 1)) != 0)
        write(out_fifo, buf, ln);

    return 0;
}
//-------------------------------------------------------------------
