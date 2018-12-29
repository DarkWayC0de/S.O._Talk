#include <iostream>
#include "../include/socket.h"
#include <string>
#include <thread>



//toca la pag 21  ojo a los semaforos
sockaddr_in make_ip_address(const char* ip_address, int port);


int main() {
    system("clear");
    std::thread my_thead();
    try {
        Socket A(make_ip_address("192.168.1.36", 25865));

        sockaddr_in remote{};
        remote = make_ip_address("192.168.1.39", 25865);

        std::string quit("/quit");
        bool terminar = false;
        do {
            std::cout << "Escribe un mensaje para " << inet_ntoa(remote.sin_addr) << " o \"/quit\" para cerrar: ";
            std::string message_text;
            std::getline(std::cin, message_text);
            if (message_text.compare(quit) == 0) {
                terminar = true;
            } else {
                Message message{};
                //Enviar
                message_text.copy(message.text, sizeof(message.text) - 1, 0);
                A.send_to(message, remote);
                system("clear");
                std::cout<<"Esperando respuesta...\n";
                //Resivir
                A.receive_from(message, remote);
                system("clear");
                //Mostrado
                char *remote_ip = inet_ntoa(remote.sin_addr);
                int remote_port = ntohs(remote.sin_port);
                message.text[254] = '\0';
                std::cout << "El sistema " << remote_ip << ":" << remote_port << " envio el mensaje'" << message.text
                          << "'\n";
            }
        } while (!terminar);
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
