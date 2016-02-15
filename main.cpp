#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <mqueue.h>

//-------------------------------------------------------------------

#define POSIX_MSG_F_NAME "/test.mq"
//#define MSG_FILE_NAME "/home/box/message.txt"
#define MSG_FILE_NAME "/media/sf_mint/projects/qt/lessons/01/mess.txt"

#define MAX_SIZE 1024

using namespace std;
//-------------------------------------------------------------------
int main(int argc, char *argv[]) {
    mqd_t mq_key;

    struct mq_attr attr;
    attr.mq_flags = O_NONBLOCK;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;

    if( (mq_key = mq_open(POSIX_MSG_F_NAME, O_RDONLY|O_NONBLOCK|O_CREAT, 0660, &attr) ) == (mqd_t) -1 ) {
        perror("POSIX error: mq_open :");
        return 0;
    }
    char buff[MAX_SIZE];
    ssize_t res;
    while(1) {
        res = mq_receive(mq_key, buff, MAX_SIZE, NULL);
        if(res != -1) {
            int f = open (MSG_FILE_NAME, O_RDWR|O_CREAT, 0660);
            if( write(f, buff, res) == -1){
                perror("write error:");
                return 0;
            }
            close(f);
        }
    }

    return 0;
}
//-------------------------------------------------------------------
