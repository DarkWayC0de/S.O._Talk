#include <iostream>
#include <string>
#include <thread>
#include <pthread.h>
#include <atomic>
#include <sys/types.h>
#include <csignal>
#include <system_error>
#include <chrono>
#include <unistd.h>
#include <set>
#include <mutex>


#include "../include/socket.h"
#include "../include/Globasle&Funciones.h"
#include "../include/hilo1_recibir.h"
#include "../include/hilo2_enviar.h"

int main(int argc, char* argv[]) {
    std::signal(SIGINT,&int_signale_handler);
    std::signal(SIGTERM,&int_signale_handler);
    std::signal(SIGHUP,&int_signale_handler);
    std::signal(SIGABRT,&int_signale_handler);
    try {
        bool help_option=false;
        bool server_option=false;
        std::string port_option;
        std::string ip_server;
        std::string user;

        opciones(argc,argv,help_option,server_option,port_option, ip_server,user);
        if(!help_option) {
            system("clear");
            if(user.empty()){
               user =getenv("USER");
            }

            if(!server_option){
                //Cliente
                Socket A(make_ip_address("",8000));
                if(ip_server.empty()){
                    std::cout<<"Introduce la ip del servidor: ";
                    std::getline(std::cin,ip_server);
                    if(port_option.empty()){
                        std::cout<<"Introduce el puerto del servidor: ";
                        std::getline(std::cin,port_option);
                    }
                }
                sockaddr_in server{};
                if(port_option.empty()) {
                    server = make_ip_address(ip_server.data(), 8000);
                }else{
                    std::string::size_type  sz;
                    int port= std::stoi(port_option,&sz);
                    server=make_ip_address(ip_server.data(), port);
                }
                std::exception_ptr eptr{};
                std::exception_ptr eptr2{};
                std::thread recibir_msg(&recibir_msg_f, std::ref(A), std::ref(server), std::ref(eptr));
                std::thread enviar_msg(&enviar_msg_f, std::ref(A), std::ref(server), std::ref(eptr2),std::ref(user));

                while (!quit);
                std::exception_ptr eptr3{};
                std::exception_ptr eptr4{};
                request_cancellation(recibir_msg, std::ref(eptr3));
                request_cancellation(enviar_msg, std::ref(eptr4));
                if (eptr3) {
                    std::rethrow_exception(eptr3);
                }
                if (eptr4) {
                    std::rethrow_exception(eptr4);
                }

                enviar_msg.join();
                recibir_msg.join();
                if (eptr2) {
                    std::rethrow_exception(eptr2);
                }
                if (eptr) {
                    std::rethrow_exception(eptr);
                }
            }else{
                //Server
                sockaddr_in own{};
                if(port_option.empty()){
                    own=make_ip_address("",8000);
                }else{
                    std::string::size_type  sz;
                    int port= std::stoi(port_option,&sz);
                    own=make_ip_address("",port);
                }
                Socket A(own);
                std::set<std::pair<std::string,std::pair<char*,int>>> destination_addresses;
                std::mutex destination_addresses_mutex;

                std::exception_ptr eptr{};
                std::exception_ptr eptr2{};
                std::thread recibir_msg(&recibir_msg_f2, std::ref(A), std::ref(destination_addresses), std::ref(eptr),std::ref(destination_addresses_mutex));
                std::thread enviar_msg(&enviar_msg_f2, std::ref(A), std::ref(destination_addresses), std::ref(eptr2),std::ref(user),std::ref(destination_addresses_mutex));

                while (!quit);
                std::exception_ptr eptr3{};
                std::exception_ptr eptr4{};
                request_cancellation(recibir_msg, std::ref(eptr3));
                request_cancellation(enviar_msg, std::ref(eptr4));
                if (eptr3) {
                    std::rethrow_exception(eptr3);
                }
                if (eptr4) {
                    std::rethrow_exception(eptr4);
                }

                enviar_msg.join();
                recibir_msg.join();
                if (eptr2) {
                    std::rethrow_exception(eptr2);
                }
                if (eptr) {
                    std::rethrow_exception(eptr);
                }

            }
        }else{
            help();
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


