//
// Created by nikita on 02.01.2021.
//

#ifndef KP_OS_CLIENT_H
#define KP_OS_CLIENT_H
#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
enum Operation{
    Send, //0
    Receive //1
};
class Connection{
private:
    std::string address = "tcp://";
    zmqpp::context context;
    zmqpp::socket socket = zmqpp::socket(context, zmqpp::socket_type::req);
    void SendData(std::string &messageString);
    std::string ReceiveData();
public:
    friend void SendMessage(Connection cont, std::string &id, std::string &message);
    friend std::string ReceiveMessage(Connection cont, std::string &id);
    Connection& operator =(const Connection &cont);
    explicit Connection(std::string &argAddress);
    Connection() = default;
    ~Connection() = default;
    Connection(Connection &cont);
};
void SendMessage(Connection cont, std::string &id, std::string &message);
std::string ReceiveMessage(Connection cont, std::string &id);
#endif //KP_OS_CLIENT_H
