//
// Created by nikita on 02.01.2021.
//
#include "server.h"
typedef void * (*THREADFUNCPTR)(void *);

[[noreturn]] void* Server::ThreadFunc(){
    while(true){
        SendReply();
    }
}

Server::~Server(){
    pthread_cancel(thread);
}

Server::Server(int port) {
    if(ServerUp(port)){
        pthread_create(&thread, nullptr, (THREADFUNCPTR) this->ThreadFunc(), nullptr);
    }
    std::cout << "Set up succesful!\n";
}

Server::Server(int port, long long maxSize) : maxSize{maxSize} {
    if(ServerUp(port)){
        pthread_create(&thread, nullptr, (THREADFUNCPTR) this->ThreadFunc(), nullptr);
    }
    std::cout << "Set up succesful!\n";
}

bool Server::ServerUp(int port){
    try{
        this->socket.bind(this->host + std::to_string(port));
    }
    catch(zmqpp::zmq_internal_exception& ex){
        std::cout << "Port is unavailable!" << std::endl;
        return false;
    }
    std::cout << "Set up successful!" << std::endl;
    return true;
}

void Server::SendReply(){
    std::string request = ReceiveData();
    std::istringstream in(request);
    static int type;
    static std::string id;
    static std::string message;
    in >> type;
    in >> id;
    message.clear();
    if(type == Send){
        ++currentSize;
        in.get();
        getline(in, message);
        if(currentSize == maxSize){
            messageQueue[orderQueue.front()].pop();
            orderQueue.pop_front();
            --currentSize;
        }
        messageQueue[id].push(message);
        orderQueue.push_back(id);
        std::string reply = std::to_string(1);
        SendData(reply);
        return;
    }
    if(type == Receive){
        message = messageQueue[id].front();
        auto it = std::find(orderQueue.begin(), orderQueue.end(), id);
        orderQueue.erase(it);
        SendData(message);
        --currentSize;
        return;
    }
}

void Server::SendData(std::string &messageString){
    zmqpp::message message;
    message << messageString;
    try {
        this->socket.send(message);
    } catch(zmqpp::zmq_internal_exception& e) {
        message = false;
    }
}

std::string Server::ReceiveData(){
    zmqpp::message message;
    try {
        this->socket.receive(message);
    } catch(zmqpp::zmq_internal_exception& e) {
        message = false;
    }
    std::string request;
    message >> request;
    return request;
}
