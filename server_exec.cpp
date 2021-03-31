#include "server.h"

int main(int argc, char* argv[]) {
    std::string cmd;
    std::ifstream config("config.txt");
    long long size = 0;
    if(config.is_open()){
        config >> size;
    }
    config.close();
    int port;
    Server* server;
    while(true){
        std::cin >> cmd;
        if(cmd == "server.out"){
            std::cin >> port;
            server = new Server(port, size);
        }
        else if(cmd == "server.down"){
            delete server;
            break;
        }
        else{
            std::cout << "Wrong command!" << std::endl;
        }
    }
}


