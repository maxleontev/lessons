#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <fcntl.h>

//-------------------------------------------------------------------

#define MSG_F_NAME "/tmp/msg.temp"
#define MSG_FILE_NAME "/home/box/message.txt"
//#define MSG_FILE_NAME "/media/sf_mint/projects/qt/lessons/01/mess.txt"

using namespace std;
//-------------------------------------------------------------------
int main(int argc, char *argv[]) {

    key_t key;
    if ( (key = ftok(MSG_F_NAME, 1)) == (key_t) -1) {
        perror("IPC error: ftok");
        return 0;
    }

    int msg_id;
    if( (msg_id = msgget(key, IPC_CREAT | 0777)) == -1) {
        perror("IPC error: msgget");
        return 0;
    }

    struct message {
            long mtype;
            char mtext[80];
    } mess;

    int res = -1;
    while(res == -1) {
        res = msgrcv(msg_id, &mess, sizeof(message) - sizeof(long), 0, MSG_NOERROR|IPC_NOWAIT);
    }
    std::cout << "res =" << res << std::endl;

    int f = open (MSG_FILE_NAME, O_RDWR|O_CREAT, 0660);
    if( write(f, mess.mtext, res) == -1){
        perror("write error:");
        return 0;
    }
    close(f);

    return 0;
}
//-------------------------------------------------------------------
