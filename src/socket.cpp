//
// Created by darkwayc0de on 19/12/18.
//


#include "../include/socket.h"

Socket::Socket(const sockaddr_in& address) {

     fd_ = socket(AF_INET, SOCK_DGRAM, 0);

     if (fd_ < 0) {
        throw std::system_error(errno,std::system_category(),"Talk: no se pudo crear el socket");

    }else {
         result_ = bind(fd_, reinterpret_cast<const sockaddr *>(&address), sizeof(address));
         if (result_ < 0) {
             throw std::system_error(errno,std::system_category(),"Talk: fallo bind");
         }
     }

}
Socket::~Socket() {
    close(fd_);
}

void Socket::send_to(const Message &message, const sockaddr_in &address) {
    int enviar = sendto(fd_, &message, sizeof(message),0,reinterpret_cast<const sockaddr*>(&address),sizeof(address));
    if(enviar<0){
        throw std::system_error(errno,std::system_category(),"Talk: fallo sendto");
    }
}

void Socket::receive_from(Message &message, sockaddr_in &address) {
    socklen_t src_len= sizeof(address);
    int resivir=recvfrom(fd_,&message,sizeof(message),0,reinterpret_cast<sockaddr*>(&address),&src_len);
    if(resivir<0){
        throw std::system_error(errno,std::system_category(),"Talk: fallo recvfrom");
    }
}