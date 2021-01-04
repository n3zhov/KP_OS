//
// Created by nikita on 02.01.2021.
//
#include"client.h"
void menu(){
    std::cout << "Enter 1 if you want to connect to server" << std::endl;
    std::cout << "Enter 2 if you want to send little message (single line, under 256 symbols)" << std::endl;
    std::cout << "Enter 3 if you want to send big message" << std::endl;
    std::cout << "Enter 4 if you want to receive messages from queue" << std::endl;
    std::cout << "Enter 5 to see this menu" << std::endl;
    std::cout << "Enter 6 to exit from program" << std::endl;
}
int main(int argc, char* argv[]){
    if(argc == 2){
        SetDir(argv[1]);
    }
    int cmd;
    menu();
    while(true){
        std::cin >> cmd;
        switch(cmd){
            case 1:{
                int port;
                std::string address;
                std::cout << "Enter IP address and port to connect" << std::endl;
                std::cin >> address >> port;
                if(ConnectToServer(address, port)){
                    std::cout << "Connection successful!" << std::endl;
                }
                else{
                    std::cout << "Error!" << std::endl;
                }
                break;
            }
            case 2:{
                std::cout << "Enter id of queue and message:" << std::endl;
                int id;
                std::string message;
                std::cin >> id;
                std::cin.get();
                getline(std::cin, message);
                if(sendSocket){
                    SendMessage(id, message);
                    std::cout << "Message delivered!" << std::endl;
                }
                else{
                    std::cout << "Something wrong with connection!" << std::endl;
                }
                break;
            }
            case 3:{
                std::cout << "Enter id of queue and name of file where message placed:" << std::endl;
                int id;
                std::string filename;
                std::cin >> id;
                std::cin.get();
                getline(std::cin, filename);
                if(sendSocket){
                    SendBigMessage(id, filename);
                    std::cout << "Message delivered!" << std::endl;
                }
                else{
                    std::cout << "Something wrong with connection!" << std::endl;
                }
                break;
            }
            case 4:{
                std::cout << "Enter id of queue" << std::endl;
                int id;
                std::cin >> id;
                RequestMessages(id);
                std::cout << "Check the message dir, if you specified one, or check the current filepath" << std::endl;
                break;
            }
            case 5:{
                menu();
                break;
            }
            case 6:{
                return 0;
            }
            default:{
                std::cout << "Wrong command!" << std::endl;
                break;
            }
        }
    }
}