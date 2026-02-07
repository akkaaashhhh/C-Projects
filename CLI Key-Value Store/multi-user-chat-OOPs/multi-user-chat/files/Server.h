#pragma once
#include "Client.h"
#include<utility>
#include<vector>
#include<mutex>

class Server{
    private : 
        int listenSock;
        int port;
        int curClientId;

        std::vector<Client> clients;
        std::vector<std::pair<int,int>> messageQueue;
        std::mutex mtx;

        void handleClient(int client);
        void acceptClient();
        void closeClient(int client);
    public : 
        Server(int PortNumber);
        void run();
        void broadcast(std::string message , int clientId);
        void loadMessageQueue(int clientId);
};