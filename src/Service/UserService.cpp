//
// Created by syh on 10/9/18.
//
#include "UserService.h"

// 静态变量声明
unordered_map<string, User> UserService::users;

/**
 * 请求控制
 * @param request 客户端请求
 * @param fd 文件描述符
 */
void UserService::process_rq(const vector<string> &request, int fd) {
    if (request[1] == "login") {
        // uno01 login username\r\n
        login(request[2].substr(0, request[2].size() - 2), fd);
    }
}

/**
 * 处理用户登录
 * @param username 用户名
 * @param fd 文件描述符
 * @return 发送的字节数
 */
int UserService::login(const string &username, int fd) {
    string msg = "uno01 login " + username;
    TimeUtil timeUtil;
    printf("[%s] ", timeUtil.getTimeInMillis().c_str());
    if (users.find(username) != users.end()) { // 该用户已注册
        msg += " 0\r\n";
        printf("UserService: user %s cannot be registered again\n", username.c_str());
    } else { // 该用户未注册
        msg += " 1\r\n";
        printf("UserService: user %s got registered\n", username.c_str());
        User user(username, fd);
        users[username] = user;
    }
    return unicast(fd, msg.c_str());
}

const unordered_map<string, User> &UserService::getUsers() {
    return users;
}

void UserService::setUsers(const unordered_map<string, User> &users) {
    UserService::users = users;
}
