//
// Created by darkwayc0de on 31/12/18.
//

#ifndef TALK_HILO2_ENVIAR_H
#define TALK_HILO2_ENVIAR_H
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

        do {
            std::cout << "\nEscribe un mensaje para " << inet_ntoa(remote.sin_addr) << " o \"/quit\" para cerrar: ";
            std::string message_text;
            std::getline(std::cin, message_text);
            if (message_text.compare(quit2) == 0) {
                terminar= true;
            } else {
                Message message{};
                //Enviar

                user.copy(message.user, sizeof(message.user)-1,0);
                message_text.copy(message.text, sizeof(message.text) - 1, 0);
                A.send_to(message, remote);

            }
        } while ((!terminar)&&(!quit));

    }catch (std::exception){
        eptr=std::current_exception();
    }
    quit=true;
}
//server
void enviar_msg_f2(Socket A,std::set<std::pair<std::string,std::pair<char*,int>>> &destination_addresses, std::exception_ptr &eptr,std::string user){
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

        do {
            if(!destination_addresses.empty()) {
                std::cout << "\nEscribe un mensaje para enviar al chat o \"/quit\" para cerrar: ";

                std::string message_text;
                std::getline(std::cin, message_text);
                if (message_text.compare(quit2) == 0) {
                    terminar = true;
                } else {
                    Message message{};
                    //Enviar

                    user.copy(message.user, sizeof(message.user)-1,0);
                    message_text.copy(message.text, sizeof(message.text) - 1, 0);
                    for(std::set<std::pair<std::string,std::pair<char*,int>>>::iterator i=destination_addresses.begin();i!=destination_addresses.end();i++){
                        remote=make_ip_address(i->second.first,i->second.second);
                        A.send_to(message,remote);
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
