//
// Created by nikita on 02.01.2021.
//

#ifndef KP_OS_CLIENT_H
#define KP_OS_CLIENT_H
#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
enum Operation{
    Send, //0
    BigSend, //1
    Receive //2
};
const std::string host = "tcp://127.0.0.1:";
std::string dirName = "";
zmqpp::context context;
zmqpp::socket sendSocket(context, zmqpp::socket_type::req);
zmqpp::socket receiveSocket(context, zmqpp::socket_type::pull);
void SendMessage(int id, std::string &message);
void SendDirName(std::string &name);
void SendData(std::string &messageString);
std::string ReceiveData();
void SendBigMessage(int id, std::string &filename);
void RequestMessages(int id);
bool ConnectToServer(std::string &address, int port);
int BindSocket(zmqpp::socket &socket);
void ReceiveMessages();
#endif //KP_OS_CLIENT_H
