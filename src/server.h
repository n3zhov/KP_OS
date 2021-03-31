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
#include <queue>
#include <map>
#include <vector>
#include <algorithm>
enum Operation{
    Send, //0
    Receive //1
};

class Server{
private:
    const std::string host = "tcp://127.0.0.1:";
    long long maxSize = 100;
    long long currentSize = 0;
    void SendData(std::string &messageString);
    std::string ReceiveData();
    pthread_t thread;
    [[noreturn]] void* ThreadFunc();
    zmqpp::context context;
    zmqpp::socket socket = zmqpp::socket(context, zmqpp::socket_type::rep);
    std::map<std::string, std::queue<std::string>> messageQueue;
    //Очередь для хранения порядка добавления сообщений
    std::list<std::string> orderQueue;
    bool ServerUp(int port);
    void SendReply();
public:
    Server(int port);
    Server(int port, long long maxSize);
    ~Server();
};

#endif //KP_OS_SERVER_EXEC_H
