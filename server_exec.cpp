#include "server.h"

struct ThreadToken{
    std::ofstream *log;
};

[[noreturn]] void* ThreadFunc(void* token){
    auto* tok = (ThreadToken*) token;
    while(true){
        SendReply(reinterpret_cast<std::ofstream &>(tok->log));
    }
}
int main(int argc, char* argv[]) {
    std::string cmd;
    if(argc < 2){
        std::cout << "Wrong count of arguments!" << std::endl;
        return 0;
    }
    if(argc == 3){
        maxSize = std::stoi(argv[2]);
    }
    auto *log = new std::ofstream(argv[1]);
    auto *log_in = new std::ifstream(argv[1]);
    ThreadToken token;
    token.log = log;
    pthread_t thread;
    int port;
    while(true){
        std::cin >> cmd;
        if(cmd == "server.out"){
            std::cin >> port;
            ServerUp(port, reinterpret_cast<std::ifstream &>(log_in));
            pthread_create(&thread, nullptr, ThreadFunc, &token);
        }
        else if(cmd == "server.down"){
            log->close();
            log_in->close();
            return 0;
        }
        else{
            std::cout << "Wrong command!" << std::endl;
        }
    }
}
