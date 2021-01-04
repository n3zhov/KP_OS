#include "server.h"

struct ThreadToken{
    std::ofstream *log;
};

[[noreturn]] void* ThreadFunc(void* token){
    auto* tok = (ThreadToken*) token;
    while(true){
        SendReply(((*tok->log)));
    }
}
int main(int argc, char* argv[]) {
    std::string cmd;
    if(argc < 2){
        std::cout << "Wrong count of arguments!" << std::endl;
        return 0;
    }
    std::ifstream config("config.txt");
    int size = 0;
    if(config.is_open()){
        config >> size;
        SetMaxSize(size);
    }
    config.close();
    auto *log = new std::ofstream(argv[1], std::ios::app);
    auto *log_in = new std::ifstream(argv[1]);
    ThreadToken token;
    token.log = log;
    pthread_t thread;
    int port;
    while(true){
        std::cin >> cmd;
        if(cmd == "server.out"){
            std::cin >> port;
            ServerUp(port, ((*log_in)));
            pthread_create(&thread, nullptr, ThreadFunc, &token);
        }
        else if(cmd == "server.down"){
            pthread_cancel(thread);
            log->close();
            log_in->close();
            return 0;
        }
        else{
            std::cout << "Wrong command!" << std::endl;
        }
    }
}


