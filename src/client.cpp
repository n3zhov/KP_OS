//
// Created by nikita on 02.01.2021.
//
#include "client.h"
int BindSocket(zmqpp::socket &socket){
    int port = 3000;
    while (true) {
        try {
            socket.bind(host+std::to_string(port));
            break;
        } catch(zmqpp::zmq_internal_exception& e) {
            ++port;
        }
    }
    return port;
}

bool ConnectToServer(std::string &address, int port){
    std::string connect = "tcp://" + address + ":" + std::to_string(port);
    try{
        sendSocket.connect(connect);
    }
    catch(zmqpp::zmq_internal_exception& e){
        std::cout << "Internal exception!" << std::endl;
        return false;
    }
    return true;
}

void SendData(std::string &messageString){
    zmqpp::message message;
    message >> messageString;
    try {
        sendSocket.send(message);
    } catch(zmqpp::zmq_internal_exception& e) {
        message = false;
    }
}

void SendMessage(int id, std::string &message){
    std::string request = std::to_string(Send) + " " + std::to_string(id) + " " + message;
    SendData(request);
    std::string reply = ReceiveData();
}

void SendBigMessage(int id, std::string &filename){
    std::string request = std::to_string(BigSend) + " " + std::to_string(-1);
    SendData(request);

    int messageId;
    std::string reply = ReceiveData();
    std::istringstream getId(reply);
    getId >> messageId;
    std::ifstream in;
    try{
        in.open(filename);
    }
    catch(...){
        std::cout << "Error opening file!" << std::endl;
        return;
    }
    static char c;
    static std::string buf;
    while(c != EOF){
        int count = 0;
        c = 0;
        buf.clear();
        while(count < 256){
            c = (char)in.get();
            if(c != EOF) {
                buf += c;
            }
            else {
                break;
            }
            ++count;
        }
        if(c != EOF) {
            c = 0;
        }
        std::string message = std::to_string(messageId) + " " + buf;
        SendData(message);
        reply = ReceiveData();
    }
}

void RequestMessages(int id){
    std::string request = std::to_string(Receive) + " " + std::to_string(id);
}

std::string ReceiveData(){
    zmqpp::message message;
    try {
        sendSocket.receive(message);
    } catch(zmqpp::zmq_internal_exception& e) {
        message = false;
    }
    std::string request;
    message >> request;
    return request;
}

void SetDirName(std::string &name){
    dirName = name;
}