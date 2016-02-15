#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct message {
    long mtype;
    char mtext[80];
};

int main(){
    key_t key = ftok("/tmp/msg.temp", 1);
    int id = msgget(key, 0666);
    struct message m;
    m.mtype = 1;
    strcpy(m.mtext, "Hello, world!");
    msgsnd(id, (struct msgbuf *) &m, 13, 0);
    return 0;
}
