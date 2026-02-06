#pragma once

#include "Client.h"
#include <vector>
#include <mutex>
#include <string>



class Server
{
private:
    int listenSock;
    int port;

    std::vector<Client> clients;
    std::mutex mtx;               
    int nextId;                   


    void acceptClient(); 
    void handleClient(Client client);
    void removeClient(int id); 


public:
    Server(int portNumber);
    void run(); 

    void broadcast(const std::string& msg, int senderId);
};