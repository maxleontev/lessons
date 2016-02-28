#include <netinet/in.h>
#include <arpa/inet.h>
#include <ev.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <thread>

#include "html_parcer.h"
#define BUFF_SIZE 1024*1024

//-------------------------------------------------------------------
void readCallBack(struct ev_loop *loop, struct ev_io *watcher, int revents) {
    char mainDataBuffer[BUFF_SIZE];
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
        ev_loop_destroy(loop);
    }
}
//-------------------------------------------------------------------
void clientThread(struct ev_io *watcher ) {
    int clientSockDescr = accept(watcher->fd, 0, 0);

    struct ev_io *clientWatcher = (struct ev_io*) malloc(sizeof(struct ev_io));
    ev_io_init(clientWatcher, readCallBack, clientSockDescr, EV_READ);

    struct ev_loop *threadLoop = ev_loop_new(EVFLAG_AUTO);
    ev_io_start(threadLoop, clientWatcher);
    ev_run(threadLoop, 0);
}
//-------------------------------------------------------------------
void acceptCallback(struct ev_loop *loop, struct ev_io *watcher, int revents) {
    std::thread thr(clientThread, watcher);
    thr.detach();
}
//-------------------------------------------------------------------

int mainSockDescr;
//-------------------------------------------------------------------
void term_handler(int i)
{
    shutdown(mainSockDescr, SHUT_RDWR);
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
        return 0;
    }

    if(daemon) {
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
    }

    if( (mainSockDescr = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1 ) {
        return 0;
    }

    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(ip_port);
    addr.sin_addr.s_addr = inet_addr(ip_addr.c_str());

    if( bind(mainSockDescr, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        return 0;
    }

    if( listen(mainSockDescr, SOMAXCONN) == -1) {
        return 0;
    }

    struct ev_loop *mainLoop = ev_default_loop(0);
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
