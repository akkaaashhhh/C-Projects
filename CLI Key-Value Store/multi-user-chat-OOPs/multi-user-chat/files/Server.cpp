
#include<iostream>
#include<vector>
#include<thread>
#include<mutex>
#include<algorithm>
#include<cstring>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string>
#include "String.h"


Server::Server(int PortNumber) : port(portNumber),curClientId(1){
    listenSock = socket(AF_INET,SOCK_STREAM,1);

    sockaddr_in listenSocketAddress{};
    listenSocketAddress.sin_family=AF_INET;
    listenSocketAddress.sin_port=htons(portNumber);
    listenSocketAddress.sin_addr.s_addr= INADDR_ANY;

    if(::bind(listenSock, (struct sockaddr*) &listenSocketAddress, sizeof(listenSocketAddress), 0)){
        perror("bind");
        exit(1);
    }

    listen(port,10);
    
    std::cout<<"Listening on port :  "<<port<<endl;
}

void Server::run(){
    while(true){
        acceptClient();
    }
}

Server::acceptClient(){
    int clientSock = accept(listenSock, nullptr, nullptr);
    if(clientSock<0) return;

    Client client(ClientSock,nextId++);

    {
        std::lock_guard<std::mutex> lock(mtx);
        clients.push_back(client);
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        loadMessageQueue(client.getId());
    }

    std::cout<<"Client "<<client.getId()<<" Connected\n";

    std::thread(&Server::handleClient,this,client).detach();
}

Server::handleClient(int client){
    std::string message;
    while(true){
        if(!client.receive(message))break;

        std::string formatted = "Client "+std::to_string(client.get_id()) + " : " +message+"\n";

        std::cout<<formatted;

        broadcast(formatted, client.getId());

        messages.push_back({client.getId(),message});
    }
}
Server::broadcast(std::string message , int clientId){
    for(auto client : clients)
        if(client!=clientId)
            send(client,message.c_str(), strlen(message.c_str()),0);
}

Server::loadMessageQueue(int clientId){
    for(auto {client,message} : messages){
        std::string formatted = "Client "+std::to_string(client.get_id()) + " : " +message+"\n";
        {   
            std::lock_guard<std::mutex> lock(mtx);
            send(clientId, formatted.c_str(), strlen(formatted.c_str()), 0);
        }
    }
}
Server::closeClient(int client){
    std::lock_guard<std::mutex> lock(mtx);

    clients.erase(
                std::remove_if(clients.begin(), clients.end(),
            [&](const Client& c)
            {
                return c.getId() == id;
            }),
        clients.end());
}