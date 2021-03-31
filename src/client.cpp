//
// Created by nikita on 02.01.2021.
//
#include "client.h"

Connection::Connection(std::string &argAddress){
    address += argAddress;
    try{
        socket.connect(address);
    }
    catch(zmqpp::zmq_internal_exception& e){
        std::cout << "Internal exception!" << std::endl;
        return;
    }
}

void SendMessage(Connection cont, std::string &id, std::string &message) {
    std::string request = std::to_string(Send) + " " + id + " " + message;
    cont.SendData(request);
    std::string reply = cont.ReceiveData();
}

std::string ReceiveMessage(Connection cont, std::string &id) {
    std::string request = std::to_string(Receive) + " " + id;
    cont.SendData(request);
    std::string reply = cont.ReceiveData();
    return reply;
}

void Connection::SendData(std::string &messageString){
    zmqpp::message message;
    message << messageString;
    try {
        this->socket.send(message);
    } catch(zmqpp::zmq_internal_exception& e) {
        message = false;
    }
}

std::string Connection::ReceiveData(){
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

Connection & Connection::operator=(const Connection &cont) {
    this->address = cont.address;
    try{
        this->socket.connect(address);
    }
    catch(zmqpp::zmq_internal_exception& e){
        std::cout << "Internal exception!" << std::endl;
    }
    return *this;
}

Connection::Connection(Connection &cont) {
    this->address = cont.address;
    try{
        this->socket.connect(address);
    }
    catch(zmqpp::zmq_internal_exception& e){
        std::cout << "Internal exception!" << std::endl;
    }
}
