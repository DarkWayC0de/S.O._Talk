//
// Created by darkwayc0de on 31/12/18.
//

#ifndef TALK_HILO2_ENVIAR_H
#define TALK_HILO2_ENVIAR_H

void enviar_msg_f(Socket A, sockaddr_in remote, std::exception_ptr &eptr){
    try{
        std::string quit("/quit");

        do {
            std::cout << "\nEscribe un mensaje para " << inet_ntoa(remote.sin_addr) << " o \"/quit\" para cerrar: ";
            std::string message_text;
            std::getline(std::cin, message_text);
            if (message_text.compare(quit) == 0) {
                terminar= true;
            } else {
                Message message{};
                //Enviar
                message_text.copy(message.text, sizeof(message.text) - 1, 0);
                A.send_to(message, remote);

            }
        } while (!terminar);

    }catch (...){
        eptr=std::current_exception();
    }
}

#endif //TALK_HILO2_ENVIAR_H
