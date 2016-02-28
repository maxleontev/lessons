#include <stdio.h>
#include <netinet/in.h>
#include <ev.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>

#include "html_parcer.h"
//-------------------------------------------------------------------
#define BUFF_SIZE 1024*1024
char mainDataBuffer[BUFF_SIZE];

//-------------------------------------------------------------------
void readCallBack(struct ev_loop *loop, struct ev_io *watcher, int revents) {
    ssize_t recv_s = recv(watcher->fd, mainDataBuffer, BUFF_SIZE, MSG_NOSIGNAL);
    if(recv_s < 0) {
        return;
    } else if(recv_s == 0) {
        ev_io_stop(loop, watcher);
        free(watcher);
        return;
    } else {
        int snd_s = HtmlParcer(mainDataBuffer, recv_s);
        send(watcher->fd, mainDataBuffer, snd_s, MSG_NOSIGNAL);
        ev_io_stop(loop, watcher);
        close(watcher->fd);
        free(watcher);
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

int mainSockDescr;

//-------------------------------------------------------------------
void term_handler(int i)
{
    shutdown(mainSockDescr, SHUT_RDWR);
    if(i == SIGINT)
        printf ("Terminating by SIGINT.\n");
    else if(i == SIGTERM)
        printf ("Terminating by SIGTERM.\n");
    else
        printf ("Terminating.\n");
    exit(EXIT_SUCCESS);
}
//-------------------------------------------------------------------
int main(int argc, char **argv)
{
    htmlFilesDir = DEFAULT_FILES_DIR;

    struct ev_loop *mainLoop = ev_default_loop(0);

    if( (mainSockDescr = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1 ) {
        perror("socket() error : ");
        return 0;
    }

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if( bind(mainSockDescr, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind() error : ");
        return 0;
    }

    if( listen(mainSockDescr, SOMAXCONN) == -1) {
        perror("listen() error : ");
        return 0;
    }

    struct ev_io acceptWatcher;
    ev_io_init(&acceptWatcher, acceptCallback, mainSockDescr, EV_READ);
    ev_io_start(mainLoop, &acceptWatcher);



    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = term_handler;
    sigset_t newset;
    sigemptyset(&newset);
    sigaddset(&newset, SIGTERM);
    sigaddset(&newset, SIGINT);
    sa.sa_mask = newset;
    sigaction(SIGTERM, &sa, 0);
    sigaction(SIGINT, &sa, 0);


    while(1) {
        ev_loop(mainLoop, 0);
    }

    return 0;
}
//-------------------------------------------------------------------
