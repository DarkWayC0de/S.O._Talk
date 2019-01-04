//
// Created by darkwayc0de on 31/12/18.
//

#ifndef TALK_HILO1_RECIVIR_H
#define TALK_HILO1_RECIVIR_H

//Cliente
void recibir_msg_f(Socket A, sockaddr_in &remlote,std::exception_ptr &eptr){
    sigset_t set;
    sigaddset(&set,SIGINT);
    sigaddset(&set,SIGTERM);
    sigaddset(&set,SIGHUP);
    sigaddset(&set,SIGABRT);
    pthread_sigmask(SIG_BLOCK,&set, nullptr);

    try {
        sockaddr_in remote{};
        remote=make_ip_address("",8001);
        do {
            //Resibir
            Message message{};

            A.receive_from(message, remote);
            //Mostrado
            char *remote_ip = inet_ntoa(remote.sin_addr);
            int remote_port = ntohs(remote.sin_port);
            message.text[254] = '\0';
            message.user[24]='\0';
            std::cout <<"\n"<<message.user<< ":" << message.text
                      << "\n";
        }while(!quit);
    }catch(std::exception){
        eptr=std::current_exception();
    }
    quit=true;
}
int operator!=(const sockaddr_in &a, const sockaddr_in &b);
void reenviar(std::set<std::pair<std::string,std::pair<char*,int>>> &destination_addresses, Message message,sockaddr_in remote,Socket A);
void recibir_msg_f2(Socket A, std::set<std::pair<std::string,std::pair<char*,int>>> &destination_addresses,std::exception_ptr &eptr){
    sigset_t set;
    sigaddset(&set,SIGINT);
    sigaddset(&set,SIGTERM);
    sigaddset(&set,SIGHUP);
    sigaddset(&set,SIGABRT);
    pthread_sigmask(SIG_BLOCK,&set, nullptr);

    try {

        do {
            //Resibir
            Message message{};
            sockaddr_in remote {};
            A.receive_from(message, remote);
            //Mostrado
            char *remote_ip = inet_ntoa(remote.sin_addr);
            int remote_port = ntohs(remote.sin_port);
            message.text[254] = '\0';
            std::cout << "\nEl sistema " << remote_ip << ":" << remote_port << "("<<message.user<<") "<<"envio el mensaje '" << message.text
                      << "'\n";
            std::pair<std::string,std::pair<char*,int>> aux;
            aux.first=message.user;
            aux.second.first=remote_ip;
            aux.second.second=remote_port;
            destination_addresses.insert(aux);
            //reenviar(destination_addresses,message,remote,A); /no esntiedno porque no funciona la funcion
            sockaddr_in destino{};
            for(std::set<std::pair<std::string,std::pair<char*,int>>>::iterator i=destination_addresses.begin();i!=destination_addresses.end();i++){
                destino=make_ip_address(i->second.first,i->second.second);
                if(destino!=remote){
                    A.send_to(message,destino);
                }
            }
        }while(!quit);
    }catch(std::exception){
        eptr=std::current_exception();
    }
    quit=true;
}

void reenviar(std::set<std::pair<std::string,std::pair<char*,int>>> &destination_addresses, Message message,sockaddr_in remote,Socket A){
    /*sockaddr_in destino{};
    for(std::set<std::pair<std::string,std::pair<char*,int>>>::iterator i=destination_addresses.begin();i!=destination_addresses.end();i++){
        destino=make_ip_address(i->second.first,i->second.second);
        if(destino!=remote){
           A.send_to(message,destino);
        }
    }*/
}
int operator!=(const in_addr &a, const in_addr &b);
int operator!=(const sockaddr_in &a, const sockaddr_in &b) {
    if(b.sin_addr!=a.sin_addr) return 1;
    if(b.sin_family!=a.sin_family)return 1;
    if(b.sin_port!=a.sin_port)return 1;
    if(b.sin_zero!=a.sin_zero)return 1;
    return 0;
}
int operator!=(const in_addr &a, const in_addr &b){
    if(a.s_addr!=b.s_addr)return 1;
    return 0;
}

#endif //TALK_HILO1_RECIVIR_H
