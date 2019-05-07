//
// Created by darkwayc0de on 1/01/19.
//

#ifndef TALK_GLOBASLE_FUNCIONES_H
#define TALK_GLOBASLE_FUNCIONES_H

//Globales
std::atomic_bool quit(false);

//Funciones

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
void request_cancellation(std::thread& thread,std::exception_ptr &eptr){
    try {
        pthread_cancel(thread.native_handle());

    }catch(...){
        eptr=std::current_exception();
    }
}

void int_signale_handler(int signum){
    quit=true;
}
void opciones(int argc, char* argv[],bool& help_option,bool& server_option,std::string &Port_option,std::string &IP_server, std::string &user){
    int c;
    bool cliente= false;
    while((c=getopt(argc,argv,"hsu:p:c:"))!=-1&&!help_option){
        switch (c){
            case 'h':
                help_option=true;
                break;
            case 's':
                if (cliente){
                    std::cout<<"\nERROR Se ha introducido a la ves cliente y servidor";
                    help_option=true;
                }else {
                    server_option = true;
                }
                break;
            case 'c':
                    if (server_option){
                        std::cout<<"\nERROR Se ha introducido a la ves cliente y servidor";
                        help_option=true;
                    }else{
                        IP_server=std::string(optarg);
                        cliente=true;
                    }
                break;
            case 'p':
                Port_option=std::string(optarg);
                break;
            case  'u':
                user=std::string(optarg);
                break;
            case '?':
                help_option=true;
                break;
            default:
                std::fprintf(stderr,"?? getopt devolvio codigo de error 0%o ??\n",c);
                help_option=true;
                break;
        }
    }

}
void help(){
    std::cout<<"\n________TALK________\n"
               "\n"
               "Opciones:\n"
               "\t-h\t\t\t\t\t\t\tImpresión de la ayuda (este mensaje) y cerrar\n"
               "\t-c[Ip_servidor]\t\t\t\tModo cliente conectando a Ip_servidor (valor predeterminado: 10.2.1.8)\n"
               "\t-p[Puerto]\t\t\t\t\tAsignar puerto servidor (valor predeterminado: 8000)\n"
               "\t-s\t\t\t\t\t\t\tModo servidor\n"
               "\t-u[Usuario]\t\t\t\t Iniciar con un nombre de Usuario\n"
               "\n"
               "NO SE PUEDE SELECIONAR -c y -s a la vez\n"
               "Si no se introduce ninguna opcion, se inicia en modo cliente y pide ip del servidor\n";
}

#endif //TALK_GLOBASLE_FUNCIONES_H
