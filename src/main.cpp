#include <cstdio>
#include "socklib.h"
#include "Service/UserService.h"
#include "Service/GameService.h"
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

time_t server_started;

int server_bytes_sent;
int server_requests;

// 互斥量
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;;

// 函数声明

void setup(pthread_attr_t *attrp);

void *handle_call(void *fdptr);

void process_rq(char *request, int fd);

int sock;

int main(int ac, char *av[]) {
    void closeSocket(int);
    int fd;
    int *fdptr;
    pthread_t worker;
    pthread_attr_t attr;

    if (ac == 1) {
        fprintf(stderr, "usage: UNOServer portnum\n");
        exit(1);
    }

    sock = make_server_socket(atoi(av[1]));
    if (sock == -1) {
        perror("making socket");
        exit(2);
    }

    setup(&attr);
    printf("UNOServer has started\n");

    signal(SIGINT, closeSocket);

    // 主循环，接收请求，以新线程处理请求
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        fd = accept(sock, nullptr, nullptr);

        pthread_mutex_lock(&mutex);
        server_requests++;
        pthread_mutex_unlock(&mutex);

        if (fd == -1) {
            perror("UNOServer: socket connection failed");
            continue;
        }
        fdptr = (int *) malloc(sizeof(int));
        *fdptr = fd; // 作为参数传给线程
        int ret = pthread_create(&worker, &attr, handle_call, fdptr);
        if (ret != 0) {
            perror("UNOServer: thread create failed");
            exit(2);
        }
        // pthread_join(worker, nullptr);
        // close(fd);
    }
#pragma clang diagnostic pop
}

// 函数实现

void setup(pthread_attr_t *attrp) {
    pthread_attr_init(attrp);
    pthread_attr_setdetachstate(attrp, PTHREAD_CREATE_DETACHED);

    time(&server_started);
    server_requests = 0;
    server_bytes_sent = 0;
}

/**
 * 处理请求，由多线程执行
 * @param fdptr 文件描述符指针
 * @return
 */
void *handle_call(void *fdptr) {
    // TODO 处理完成后销毁线程?
    // TODO 客户端退出后关闭 socket
//    FILE *fpin;
    char request[BUFSIZ];
    int fd;

    fd = *(int *) fdptr;
    free(fdptr); // 由参数获取文件描述符

//    fpin = fdopen(fd, "r"); // 缓冲输入
//    while (1) {// 读取客户端请求
//        bzero(request, BUFSIZ);
//        if ((fgets(request, BUFSIZ, fpin)) > 0) {
//            printf("Got a call on %d: request = %s", fd, request);
//            process_rq(request, fd); // 处理客户端请求}
//        }
//    }
//    fclose(fpin);
//    return nullptr;

    while (true) {
        bzero(request, BUFSIZ);
        ssize_t len = recv(fd, request, BUFSIZ, 0);
        if (len < 0) {
            printf("Receive data failed\n");
            exit(1);
        } else if (len > 0) {
            printf("Got a call on %d: request = %s", fd, request);
            process_rq(request, fd); // 处理客户端请求}
        }
    }
}

void process_rq(char *request, int fd) {
    if (strcmp(request, "") == 0)
        return;
    UserService userService;
    GameService gameService;
    vector<string> splitStr;
    string rq_str = request;
    split(splitStr, rq_str, boost::is_any_of(" "));
    if (splitStr[0] == "uno01")
        userService.process_rq(splitStr, fd);
    else if (splitStr[0] == "uno02")
        gameService.process_rq(splitStr, fd);
    else {
        throw "process_rq: request split exception";
    }
}

void closeSocket(int signum) {
#ifndef CLOSESOCKET
    if (sock != -1) {
        shutdown(sock, SHUT_RDWR);
        printf("Socket shutdown\n");
    }
#endif
}