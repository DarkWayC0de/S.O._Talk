#include <iostream>
#include <string>
#include <thread>
#include <pthread.h>
#include <atomic>
#include <sys/types.h>
#include <csignal>
#include <system_error>
#include <chrono>

#include "../include/socket.h"
#include "../include/Globasle&Funciones.h"
#include "../include/hilo1_recibir.h"
#include "../include/hilo2_enviar.h"

//ojo a los semaforos

int main() {
    std::signal(SIGINT,&int_signale_handler);
    std::signal(SIGTERM,&int_signale_handler);
    std::signal(SIGHUP,&int_signale_handler);
    std::signal(SIGABRT,&int_signale_handler);
    try {
        //system("clear");
        Socket A(make_ip_address("192.168.1.35", 25865));

        sockaddr_in remote{};
        remote = make_ip_address("192.168.1.42", 25865);

        std::exception_ptr eptr {};
        std::exception_ptr eptr2 {};
        std::thread recibir_msg(&recibir_msg_f, std::ref(A), std::ref(remote), std::ref(eptr));
        std::thread enviar_msg(&enviar_msg_f, std::ref(A), std::ref(remote), std::ref(eptr2));

        while (!quit);
        std::exception_ptr eptr3 {};
        std::exception_ptr eptr4 {};
        request_cancellation(recibir_msg, std::ref(eptr3));
        request_cancellation(enviar_msg, std::ref(eptr4));
        if(eptr3){
            std::rethrow_exception(eptr3);
        }
        if(eptr4){
            std::rethrow_exception(eptr4);
        }

        enviar_msg.join();
        recibir_msg.join();
        if(eptr2){
            std::rethrow_exception(eptr2);
        }
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


