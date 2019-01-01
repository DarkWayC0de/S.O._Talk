#include <iostream>
#include "../include/socket.h"
#include "../include/Globasle&Funciones.h"
#include <string>
#include <thread>
#include "../include/hilo1_recivir.h"
#include "../include/hilo2_enviar.h"


//ojo a los semaforos

int main() {

    try {
        //system("clear");
        Socket A(make_ip_address("192.168.1.35", 25865));

        sockaddr_in remote{};
        remote = make_ip_address("192.168.1.42", 25865);

        std::exception_ptr eptr {};
        std::exception_ptr eptr2 {};
        std::thread recivir_msg(&recivir_msg_f, std::ref(A), std::ref(remote), std::ref(eptr));
        std::thread enviar_msg(&enviar_msg_f, std::ref(A), std::ref(remote), std::ref(eptr2));

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


