//
// Created by nikita on 02.01.2021.
//
#include"client.h"
void menu(){
    std::cout << "Enter 1 if you want to connect to server" << std::endl;
    std::cout << "Enter 2 if you want to send message" << std::endl;
    std::cout << "Enter 3 if you want to receive messages from queue" << std::endl;
    std::cout << "Enter 4 to see this menu" << std::endl;
    std::cout << "Enter 5 to exit from program" << std::endl;
}
Connection &cont();
int main(int argc, char* argv[]){
    int cmd;
    menu();
    Connection cont;
    while(true){
        std::cin >> cmd;
        switch(cmd){
            case 1:{
                int port;
                std::string address;
                std::cout << "Enter IP address and port to connect" << std::endl;
                std::cin >> address >> port;
                address = address + ":" + std::to_string(port);
                cont = Connection(address);
                break;
            }
            case 2:{
                std::cout << "Enter id of queue and message:" << std::endl;
                std::string id;
                std::string message;
                std::cin >> id;
                std::cin.get();
                getline(std::cin, message);
                SendMessage(cont, id, message);
                break;
            }
            case 3:{
                std::cout << "Enter id of queue" << std::endl;
                std::string id;
                std::cin >> id;
                std::string message = ReceiveMessage(cont, id);
                std::cout << message << std::endl;
                break;
            }
            case 4:{
                menu();
                break;
            }
            case 5:{
                return 0;
            }
            default:{
                std::cout << "Wrong command!" << std::endl;
                break;
            }
        }
    }
}