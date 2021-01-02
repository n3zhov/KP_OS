//
// Created by nikita on 02.01.2021.
//

#ifndef KP_OS_SERVER_EXEC_H
#define KP_OS_SERVER_EXEC_H
#include <zmqpp/zmqpp.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <queue>
enum Operation{
    Send, //0
    BigSend, //1
    Receive //2
};
struct messageId{
    int id;
    std::string filename;
    messageId(int id_, std::string filename_){
        this->id = id_;
        this->filename = filename_;
    }
};
static int messageIndex = 0;
static std::queue<messageId> messageQueue;
static zmqpp::context context;
static zmqpp::socket socket(context, zmqpp::socket_type::rep);
const std::string host = "tcp://127.0.0.1:";
bool ServerUp(int port);
std::string ReceiveRequest();
#endif //KP_OS_SERVER_EXEC_H
