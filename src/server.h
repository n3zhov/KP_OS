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
#include <cstdio>
#include <list>
enum Operation{
    Send, //0
    BigSend, //1
    Receive //2
};
struct messageId{
    int id;
    int second_id;
    std::string filename;
    messageId(int id_, int second_id_, std::string filename_){
        this->id = id_;
        this->second_id = second_id_;
        this->filename = filename_;
    }
};

static int maxSize = 1000;
static int queueSize = 0;
static int messageIndex = 0;
static std::list<messageId> messageQueue;
static zmqpp::context context;
static zmqpp::socket socket(context, zmqpp::socket_type::rep);
const std::string host = "tcp://127.0.0.1:";
bool ServerUp(int port, std::ifstream &log);
void SendMessages(zmqpp::socket &socketMessages, int id);
void SendReply(std::ofstream &log);
std::string ReceiveRequest();
void SendData(std::string &messageString);
void SendData(std::string &messageString, zmqpp::socket &socketSend);
int BindSocket(zmqpp::socket &socketMessages);
void DeleteMessage();
#endif //KP_OS_SERVER_EXEC_H
