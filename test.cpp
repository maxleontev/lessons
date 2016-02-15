#include <mqueue.h>

int main(){
    int mqd = mq_open("/test.mq", O_RDWR);
    mq_send(mqd, "Hello, world, good boy!", 23, 0);
    return 0;
}
