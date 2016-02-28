#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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
/*    if(i == SIGINT)
        printf ("Terminating by SIGINT.\n");
    else if(i == SIGTERM)
        printf ("Terminating by SIGTERM.\n");
    else
        printf ("Terminating.\n");*/
    exit(EXIT_SUCCESS);
}
//-------------------------------------------------------------------
int main(int argc, char **argv)
{
    int opt_res;
    std::string ip_addr = "";
    int ip_port = -1;
    htmlFilesDir = "";
    bool daemon = true;

    while( (opt_res = getopt(argc, argv, "h:p:d:x")) != -1) {
        if(opt_res == 'h') {
            ip_addr = optarg;
        } else if(opt_res == 'p') {
            ip_port = atoi(optarg);
        } else if(opt_res == 'd'){
            htmlFilesDir = optarg;
        } else if(opt_res == 'x'){
            daemon = false;
        }
    }
    if(ip_addr == "" || ip_port == -1 || htmlFilesDir == "") {
        printf("invalid arguments\r\n");
        return 0;
    }

    if(daemon) {
        printf("starting in daemon mode\r\n");
        if( fork() ) {
            return 0;
        }
        umask(0);
        if( setsid() < 0 )
            return 0;
        if((chdir("/")) < 0)
            return 0;
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
    } else {
        printf("starting in normal mode\r\n");
    }

    struct ev_loop *mainLoop = ev_default_loop(0);
    if( (mainSockDescr = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1 ) {
        if(!daemon)
            perror("socket() error : ");
        return 0;
    }

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(ip_port);
    addr.sin_addr.s_addr = inet_addr(ip_addr.c_str());

    if( bind(mainSockDescr, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        if(!daemon)
            perror("bind() error : ");
        return 0;
    }

    if( listen(mainSockDescr, SOMAXCONN) == -1) {
        if(!daemon)
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
