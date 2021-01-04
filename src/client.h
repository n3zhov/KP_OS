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
static std::string host = "";
static std::string dirName = "";
static zmqpp::context context;
static zmqpp::socket sendSocket(context, zmqpp::socket_type::req);
static zmqpp::socket receiveSocket(context, zmqpp::socket_type::rep);
void SendMessage(int id, std::string &message);
void SendData(std::string &messageString);
void SendData(std::string &messageString, zmqpp::socket &argSocket);
std::string ReceiveData();
void SendBigMessage(int id, std::string &filename);
void RequestMessages(int id);
bool ConnectToServer(std::string &address, int port);
void SetDir(std::string dir);
int BindSocket(zmqpp::socket &socket);
std::string ReceiveData(zmqpp::socket &argSocket);
#endif //KP_OS_CLIENT_H
