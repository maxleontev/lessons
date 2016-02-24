#include <stdio.h>
#include <netinet/in.h>
#include <ev.h>
#include <strings.h>
#include <stdlib.h>

//-------------------------------------------------------------------
void readCallBack(struct ev_loop *loop, struct ev_io *watcher, int revents) {
    char buffer[1024];
    ssize_t r = recv(watcher->fd, buffer, 1024, MSG_NOSIGNAL);
    if(r < 0) {
        return;
    } else if(r == 0) {
        ev_io_stop(loop, watcher);
        free(watcher);
        return;
    } else {
        send(watcher->fd, buffer, r, MSG_NOSIGNAL);
    }
}
//-------------------------------------------------------------------
void acceptCallback(struct ev_loop *loop, struct ev_io *watcher, int revents) {
    int clientSockDescr = accept(watcher->fd, 0, 0);

    struct ev_io *clientWatcher = (struct ev_io*) malloc(sizeof(struct ev_io));

    ev_io_init(clientWatcher, readCallBack, clientSockDescr, EV_READ);
    ev_io_start(loop, clientWatcher);
}
//-------------------------------------------------------------------
int main(int argc, char **argv)
{
    struct ev_loop *mainLoop = ev_default_loop(0);

    int sockDescr;
    if( (sockDescr = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1 ) {
        perror("socket() error : ");
        return 0;
    }

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if( bind(sockDescr, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind() error : ");
        return 0;
    }

    if( listen(sockDescr, SOMAXCONN) == -1) {
        perror("listen() error : ");
        return 0;
    }

    struct ev_io acceptWatcher;
    ev_io_init(&acceptWatcher, acceptCallback, sockDescr, EV_READ);

    ev_io_start(mainLoop, &acceptWatcher);

    while(1) {
        ev_loop(mainLoop, 0);
    }

    return 0;
}
//-------------------------------------------------------------------
