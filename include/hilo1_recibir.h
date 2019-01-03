//
// Created by darkwayc0de on 31/12/18.
//

#ifndef TALK_HILO1_RECIVIR_H
#define TALK_HILO1_RECIVIR_H


void recibir_msg_f(Socket A, sockaddr_in remote,std::exception_ptr &eptr){
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

            A.receive_from(message, remote);
            //Mostrado
            char *remote_ip = inet_ntoa(remote.sin_addr);
            int remote_port = ntohs(remote.sin_port);
            message.text[254] = '\0';
            std::cout << "\nEl sistema " << remote_ip << ":" << remote_port << " envio el mensaje'" << message.text
                      << "'\n";
        }while(!quit);
    }catch(std::exception){
        eptr=std::current_exception();
    }
    quit=true;
}



#endif //TALK_HILO1_RECIVIR_H
