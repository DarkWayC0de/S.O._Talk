#include <iostream>
#include "../include/socket.h"
#include <string>
#include <thread>
#include "../include/hilo1_recivir.h"
#include "../include/hilo2_enviar.h"


//ojo a los semaforos
sockaddr_in make_ip_address(const char* ip_address, int port);

int main() {

    try {
        //system("clear");
        Socket A(make_ip_address("192.168.1.35", 25865));
        sockaddr_in remote{};
        remote = make_ip_address("192.168.1.42", 25865);
        bool terminar=false;

        std::exception_ptr eptr {};
        std::exception_ptr eptr2 {};
        std::thread recivir_msg(&recivir_msg_f, std::ref(A), std::ref(remote), std::ref(terminar),std::ref(eptr));
        std::thread enviar_msg(&enviar_msg_f, std::ref(A), std::ref(remote),std::ref(terminar), std::ref(eptr));

        enviar_msg.join();
        if(eptr2){
            std::rethrow_exception(eptr);
        }
        recivir_msg.join();
        if(eptr){
            std::rethrow_exception(eptr);
        }
    }
    catch (std::bad_alloc& e){
        std::cerr<<"Talk : memoria insuficiente\n";
        return 3;
    }
    catch (std::system_error& e){
        std::cerr<<"Talk : "<<e.what()<<"\n";
        return 2;
    }
    catch (...){
        std::cerr<<"Talk : Error desconocido\n";
        return 1;
    }
    return 0;
}

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
