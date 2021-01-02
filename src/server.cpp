//
// Created by nikita on 02.01.2021.
//
#include "server.h"

bool ServerUp(int port){
    try{
        socket.bind(host + std::to_string(port));
    }
    catch(zmqpp::zmq_internal_exception& ex){
        std::cout << "Port is unavailable!" << std::endl;
        return false;
    }
    std::cout << "Set up successful!" << std::endl;
    return true;
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

void SendRequest(std::string &messageString){
    zmqpp::message message;
    message >> messageString;
    try {
        socket.send(message);
    } catch(zmqpp::zmq_internal_exception& e) {
        message = false;
    }
}

void SendReply(std::ofstream log){
    std::string request = ReceiveRequest();
    std::istringstream in(request);
    static int type;
    static char c;
    static int id;
    static int secondId;
    static std::string message;
    static std::string filename;
    in >> type;
    if(type == Send){
        in >> id;
        c = 0;
        while (c != EOF){
            c = (char)in.get();
            if(c != EOF)
                message += c;
        }
        c = 0;
        filename = "messages/" + std::to_string(id) + "_" + std::to_string(messageIndex) + ".txt";
        auto *uniqueId = new messageId(id, filename);
        messageQueue.push(*uniqueId);
        std::ofstream out(filename, std::ios::out);
        out << id << " " << messageIndex << "\n" << message;
        ++messageIndex;
    }
    if(type == BigSend){
        in >> id;
        in >> secondId;
        if(secondId == -1){
            secondId = messageIndex;

            filename = "messages/" + std::to_string(id) + "_" + std::to_string(secondId) + ".txt";
            std::ofstream out(filename, std::ios::out);
            out << id << " " << secondId << "\n";

            std::string send = std::to_string(messageIndex);
            SendRequest(send);
            ++messageIndex;
        }
        else{
            filename = "messages/" + std::to_string(id) + "_" + std::to_string(secondId) + ".txt";
            std::ofstream out(filename, std::ios::app);
            c = 0;
            while (c != EOF){
                c = (char)in.get();
                if(c != EOF)
                    message += c;
            }
            c = 0;
            out << message;
            out.close();
        }
    }
}
/*std::string recieve_msg(zmqpp::socket& socket) {
    zmqpp::message message;
    try {
        socket.receive(message);
    } catch(zmqpp::zmq_internal_exception& e) {
        message = false;
    }
    std::string answer;
    message >> answer;
    return answer;
}*/