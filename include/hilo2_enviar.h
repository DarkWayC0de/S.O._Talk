//
// Created by darkwayc0de on 31/12/18.
//

#ifndef TALK_HILO2_ENVIAR_H
#define TALK_HILO2_ENVIAR_H

#include <sstream>
#include <vector>
#include <sys/wait.h>

//cliente
void enviar_msg_f(Socket A, sockaddr_in remote, std::exception_ptr &eptr,std::string user){
    sigset_t set;
    sigaddset(&set,SIGINT);
    sigaddset(&set,SIGTERM);
    sigaddset(&set,SIGHUP);
    sigaddset(&set,SIGABRT);
    pthread_sigmask(SIG_BLOCK,&set, nullptr);
    try{
        bool terminar=false;
        std::string quit2("/quit");
        std::cout << "\nEscribe un mensaje o \"/quit\" para cerrar:\n";
        std::cout.flush();
        do {

            std::string message_text;
            std::getline(std::cin, message_text);
            if (message_text.compare(quit2) == 0) {
                terminar= true;
            } else {
                if(message_text[0]=='/'){
                    std::string run("/run");
                    std::istringstream cadena(message_text);
                    std::string palabra;
                    cadena>>palabra;
                    if(palabra.compare(run)==0) {
                        std::string comando, argumentos;
                        cadena >> comando;
                        cadena >> argumentos;
                        while (cadena >> palabra) {
                            argumentos += " " + palabra;
                        }
                        std::array<int, 2>tuberia;
                        int comprobar=pipe(tuberia.data());
                        if(comprobar<0){
                            throw std::system_error(errno,std::system_category(),"fallo al crear tuberia");
                        }

                        pid_t pid=fork(); //crear sub proceso
                        if(pid==0){
                            close(tuberia[0]);
                            dup2(tuberia[1],STDOUT_FILENO);
                            close(tuberia[1]);
                            if(argumentos.empty()){
                                execlp(comando.data(),comando.data(),NULL);
                            }else {
                                execlp(comando.data(), comando.data(), argumentos.data(), NULL);
                            }
                            throw std::system_error(errno,std::system_category(),"fallo en el exec*()");
                        } else{
                            if(pid>0){
                                close(tuberia[1]);
                                std::array<char,1024> buffer{};
                                read(tuberia[0],buffer.data(),buffer.size());
                                wait(NULL);
                                message_text=buffer.data();
                                std::cout<<"El resultado del comando es \n"<<message_text;
                                Message message{};
                                //Enviar

                                user.copy(message.user, sizeof(message.user)-1,0);
                                message_text.copy(message.text, sizeof(message.text) - 1, 0);
                                A.send_to(message, remote);

                            }else{
                                throw std::system_error(errno,std::system_category(),"fallo en fork()");

                            }
                        }

                    }else{
                        std::cout<<"El comando "<<message_text<<" no es reconocido\n";
                    }

                } else {
                    Message message{};
                    //Enviar

                    user.copy(message.user, sizeof(message.user) - 1, 0);
                    message_text.copy(message.text, sizeof(message.text) - 1, 0);
                    A.send_to(message, remote);
                }

            }
        } while ((!terminar)&&(!quit));

    }catch (std::exception){
        eptr=std::current_exception();
    }
    quit=true;
}
//server
void enviar_msg_f2(Socket A,std::set<std::pair<std::string,std::pair<char*,int>>> &destination_addresses, std::exception_ptr &eptr,std::string user, std::mutex &destination_addresses_mutex){
    sigset_t set;
    sigaddset(&set,SIGINT);
    sigaddset(&set,SIGTERM);
    sigaddset(&set,SIGHUP);
    sigaddset(&set,SIGABRT);
    pthread_sigmask(SIG_BLOCK,&set, nullptr);
    try{
        bool terminar=false;
        std::string quit2("/quit");

        sockaddr_in remote{};
        std::cout<<"Esperando el primer cliente\n";
        std::cout.flush();
        int cont =0;
        do {
            if(!destination_addresses.empty()) {
                if(!cont){
                    cont++;
                    std::cout << "Escribe un mensaje para enviar al chat o \"/quit\" para cerrar:\n";
                    std::cout.flush();
                }
                std::string message_text;
                std::getline(std::cin, message_text);
                if (message_text.compare(quit2) == 0) {
                    terminar = true;
                } else {
                    if(message_text[0]=='/'){
                        std::string run("/run");
                        std::istringstream cadena(message_text);
                        std::string palabra;
                        cadena>>palabra;
                        if(palabra.compare(run)==0) {
                            std::string comando, argumentos;
                            cadena >> comando;
                            cadena >> argumentos;
                            while (cadena >> palabra) {
                                argumentos += " " + palabra;
                            }
                            std::array<int, 2>tuberia;
                            int comprobar=pipe(tuberia.data());
                            if(comprobar<0){
                                throw std::system_error(errno,std::system_category(),"fallo al crear tuberia");
                            }

                            pid_t pid=fork(); //crear sub proceso
                            if(pid==0){
                                close(tuberia[0]);
                                dup2(tuberia[1],STDOUT_FILENO);
                                close(tuberia[1]);
                                if(argumentos.empty()){
                                    execlp(comando.data(),comando.data(),NULL);
                                }else {
                                    execlp(comando.data(), comando.data(), argumentos.data(), NULL);
                                }
                                throw std::system_error(errno,std::system_category(),"fallo en el exec*()");
                            } else{
                                if(pid>0){
                                    close(tuberia[1]);
                                    std::array<char,1024> buffer{};
                                    read(tuberia[0],buffer.data(),buffer.size());
                                    wait(NULL);
                                    message_text=buffer.data();
                                    std::cout<<"El resultado del comando es \n"<<message_text;
                                    Message message{};
                                    //Enviar
                                    user.copy(message.user, sizeof(message.user) - 1, 0);
                                    message_text.copy(message.text, sizeof(message.text) - 1, 0);
                                    destination_addresses_mutex.lock();
                                    for (auto i = destination_addresses.begin(); i != destination_addresses.end(); i++) {
                                        remote = make_ip_address(i->second.first, i->second.second);
                                        A.send_to(message, remote);
                                    }
                                    destination_addresses_mutex.unlock();
                                }else{
                                    throw std::system_error(errno,std::system_category(),"fallo en fork()");

                                }
                            }

                        }else{
                            std::cout<<"El comando "<<message_text<<" no es reconocido\n";
                        }

                    } else{
                        Message message{};
                        //Enviar
                        user.copy(message.user, sizeof(message.user) - 1, 0);
                        message_text.copy(message.text, sizeof(message.text) - 1, 0);
                        destination_addresses_mutex.lock();
                        for (auto i = destination_addresses.begin(); i != destination_addresses.end(); i++) {
                            remote = make_ip_address(i->second.first, i->second.second);
                            A.send_to(message, remote);
                        }
                        destination_addresses_mutex.unlock();
                    }
                }
            }
        } while ((!terminar)&&(!quit));

    }catch (std::exception){
        eptr=std::current_exception();
    }
    quit=true;
}

#endif //TALK_HILO2_ENVIAR_H
