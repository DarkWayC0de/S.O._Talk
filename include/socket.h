//
// Created by darkwayc0de on 19/12/18.
//
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cerrno>
#include <stdexcept>

#ifndef TALK_SOCKET_H
#define TALK_SOCKET_H

struct Message{

    char text[1024];

};

class Socket {
private:
    int fd_;
    int result_;

public:
    Socket(const sockaddr_in& address);
    ~Socket();

    void send_to(const Message& message, const sockaddr_in& address);
    void receive_from(Message& message, sockaddr_in& address);



};


#endif //TALK_SOCKET_H
