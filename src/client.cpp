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
    host = "tcp://" + address + ":";
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
    message << messageString;
    try {
        sendSocket.send(message);
    } catch(zmqpp::zmq_internal_exception& e) {
        message = false;
    }
}

void SendData(std::string &messageString, zmqpp::socket &argSocket){
    zmqpp::message message;
    message << messageString;
    try {
        argSocket.send(message);
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
    std::string request = std::to_string(BigSend) + " " + std::to_string(id) + " " + std::to_string(-1);
    SendData(request);

    int messageId;
    std::string reply = ReceiveData();
    std::istringstream getId(reply);
    getId >> messageId;
    std::ifstream in(filename);
    static char c;
    static std::string buf;
    while(c != EOF){
        c = 0;
        buf.clear();
        for(int i = 0; i < 256; ++i){
            c = (char)in.get();
            if(c != EOF) {
                buf += c;
            }
            else {
                break;
            }
        }
        if(c != EOF) {
            c = 0;
        }
        std::string message = std::to_string(BigSend) + " " + std::to_string(id) + " " + std::to_string(messageId) + " " + buf;
        SendData(message);
        reply = ReceiveData();
    }
}

void RequestMessages(int id){
    std::string request = std::to_string(Receive) + " " + std::to_string(id);
    SendData(request);
    std::string reply = ReceiveData();
    int port = BindSocket(receiveSocket);
    request = host+std::to_string(port);
    SendData(request);
    request = ReceiveData();
    static std::string filename;
    int messageId = 0;
    static std::string buf = "";
    static char c = 0;
    while(true){
        std::string response = ReceiveData(receiveSocket);
        std::istringstream in(response);
        in >> messageId;
        if(messageId == -1){
            std::string reply = std::to_string(1);
            SendData(reply, receiveSocket);
            break;
        }
        c = 0;
        in.get();
        buf.clear();
        for(int i = 0; i < 256; ++i){
            c = (char)in.get();
            if(c != EOF) {
                buf += c;
            }
            else {
                break;
            }
        }
        if(c != EOF) {
            c = 0;
        }
        if(!dirName.empty()) {
            filename = dirName + '/' + std::to_string(id) + "_" + std::to_string(messageId) + ".txt";
        }
        else{
            filename = std::to_string(id) + "_" + std::to_string(messageId) + ".txt";
        }
        std::ofstream out(filename, std::ios::app);
        out << buf;
        std::string reply = std::to_string(1);
        SendData(reply, receiveSocket);
    }
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

std::string ReceiveData(zmqpp::socket &argSocket){
    zmqpp::message message;
    try {
        argSocket.receive(message);
    } catch(zmqpp::zmq_internal_exception& e) {
        message = false;
    }
    std::string request;
    message >> request;
    return request;
}

void SetDir(std::string dir){
    dirName = dir;
}