//
// Created by nikita on 02.01.2021.
//

#ifndef KP_OS_CLIENT_H
#define KP_OS_CLIENT_H
#include <zmqpp/zmqpp.hpp>
enum Operation{
    Send, //0
    BigSend, //1
    Receive //2
};
#endif //KP_OS_CLIENT_H
