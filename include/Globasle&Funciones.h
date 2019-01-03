//
// Created by darkwayc0de on 1/01/19.
//

#ifndef TALK_GLOBASLE_FUNCIONES_H
#define TALK_GLOBASLE_FUNCIONES_H

//Globales
std::atomic_bool quit(false);

//Funciones

sockaddr_in make_ip_address(const char* ip_address, int port) {
    sockaddr_in local_address{};
    if(ip_address==""){
        local_address.sin_addr.s_addr = htonl(INADDR_ANY);
    }else{
        inet_aton(ip_address,&local_address.sin_addr);

    }

    local_address.sin_family = AF_INET;

    local_address.sin_port = htons(port);
    return local_address;

}
void request_cancellation(std::thread& thread,std::exception_ptr &eptr){
    try {
        pthread_cancel(thread.native_handle());

    }catch(...){
        eptr=std::current_exception();
    }
}

void int_signale_handler(int signum){
    quit=true;
}

#endif //TALK_GLOBASLE_FUNCIONES_H
