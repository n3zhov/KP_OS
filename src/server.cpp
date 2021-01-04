//
// Created by nikita on 02.01.2021.
//
#include "server.h"

bool ServerUp(int port, std::ifstream &log){
    try{
        socket.bind(host + std::to_string(port));
    }
    catch(zmqpp::zmq_internal_exception& ex){
        std::cout << "Port is unavailable!" << std::endl;
        return false;
    }
    static int id, second_id;
    static std::string filename;
    while(log){
        log >> id >> second_id >> filename;
        auto *uniqueId = new messageId(id, second_id, filename);
        messageQueue.push_back(*uniqueId);
    }
    ResizeQueue();
    messageIndex = second_id + 1;
    std::cout << "Set up successful!" << std::endl;
    return true;
}

void ResizeQueue(){
    while (messageQueue.size() > maxSize){
        DeleteMessage();
    }
}

void SendMessages(zmqpp::socket &socketMessages, int id){
    auto it = messageQueue.begin();
    auto end = messageQueue.end();
    static char c;
    static std::string buf;
    while(it != end){
        if((*it).id == id){
            std::ifstream in((*it).filename);
            c = 0;
            buf.clear();
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
                std::string message = std::to_string((*it).second_id) + " " + buf;
                SendData(message, socketMessages);
                std::string reply = ReceiveRequest(socketMessages);
            }
        }
        ++it;
    }
    std::string message = std::to_string(-1);
    SendData(message, socketMessages);
}

int BindSocket(zmqpp::socket &socketMessages){
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


std::string ReceiveRequest(){
    zmqpp::message message;
    try {
        socket.receive(message);
    } catch(zmqpp::zmq_internal_exception& e) {
        message = false;
    }
    std::string request;
    message >> request;
    return request;
}

std::string ReceiveRequest(zmqpp::socket &receiveSocket){
    zmqpp::message message;
    try {
        receiveSocket.receive(message);
    } catch(zmqpp::zmq_internal_exception& e) {
        message = false;
    }
    std::string request;
    message >> request;
    return request;
}

void SendData(std::string &messageString, zmqpp::socket &socketSend){
    zmqpp::message message;
    message << messageString;
    try {
        socketSend.send(message);
    } catch(zmqpp::zmq_internal_exception& e) {
        message = false;
    }
}

void SendData(std::string &messageString){
    zmqpp::message message;
    message << messageString;
    try {
        socket.send(message);
    } catch(zmqpp::zmq_internal_exception& e) {
        message = false;
    }
}

void DeleteMessage(){
    messageId buf = messageQueue.front();
    try{
        remove(buf.filename.c_str());
    }
    catch(...){
        std::cout << "Some error!" << std::endl;
    }
    messageQueue.pop_front();
}

void SetMaxSize(int size){
    maxSize = size;
}

void SendReply(std::ofstream &log){
    std::string request = ReceiveRequest();
    std::istringstream in(request);
    static int type;
    static char c;
    static int id;
    static int secondId;
    static std::string message;
    static std::string filename;
    in >> type;
    in >> id;
    message.clear();
    if(type == Send){
        in.get();
        getline(in, message);
        filename = "messages/" + std::to_string(id) + "_" + std::to_string(messageIndex) + ".txt";
        auto *uniqueId = new messageId(id, messageIndex, filename);
        messageQueue.push_back(*uniqueId);
        std::ofstream out(filename);
        log << id << " " << messageIndex << " " << filename << "\n";
        out << message;
        ++messageIndex;
        ++queueSize;
        if(queueSize == maxSize){
            DeleteMessage();
            --queueSize;
        }
        std::string reply = std::to_string(1);
        SendData(reply);
        return;
    }
    if(type == BigSend){
        in >> secondId;
        if(secondId == -1){
            secondId = messageIndex;

            filename = "messages/" + std::to_string(id) + "_" + std::to_string(secondId) + ".txt";
            std::ofstream out(filename);
            log << id << " " << messageIndex << " " << filename << "\n";

            auto *uniqueId = new messageId(id, messageIndex, filename);
            messageQueue.push_back(*uniqueId);

            std::string send = std::to_string(messageIndex);
            SendData(send);
            ++messageIndex;
            ++queueSize;
            if(queueSize == maxSize){
                DeleteMessage();
                --queueSize;
            }
            return;
        }
        else{
            filename = "messages/" + std::to_string(id) + "_" + std::to_string(secondId) + ".txt";
            std::ofstream out(filename, std::ios::app);
            c = 0;
            in.get();
            while (c != EOF){
                c = (char)in.get();
                if(c != EOF)
                    message += c;
            }
            c = 0;
            out << message;
            out.close();
            std::string reply = std::to_string(1);
            SendData(reply);
            return;
        }
    }
    if(type == Receive){
        auto *socketMessages = new zmqpp::socket(context, zmqpp::socket_type::req);
        std::string response = std::to_string(1);
        SendData(response);
        std::string address = ReceiveRequest();
        socketMessages->connect(address);
        response = std::to_string(1);
        SendData(response);
        SendMessages((*socketMessages), id);
    }
}