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

std::vector<int> clients;
std::mutex mtx;
std::vector<std::string> messageQueue;

void broadcast(const char* msg, int sender){
    std::lock_guard<std::mutex> lock(mtx);
    for(int c:clients)
        if(c != sender)
            send(c, msg, strlen(msg),0);
}


int FindClientID(int client){
    std::lock_guard<std::mutex> lock(mtx);

    for(int i=0;i<clients.size();i++){
        if(clients[i]==client) return i;
    }
    return -1;
}

void handleClient(int client){
    char buffer[1024];
    
    while(true){

        int n = recv(client, buffer, sizeof(buffer)-1, 0);
        if(n <= 0) break;

        buffer[n] = '\0';

        int id = FindClientID(client);

        std::string msg =
            "Client " + std::to_string(id+1) + ": " + buffer + "\n";

        std::cout << msg << std::endl;

        {
            std::lock_guard<std::mutex> lock(mtx);
            messageQueue.push_back(msg);
        }

        broadcast(msg.c_str(), client);
    }

    close(client);

    std::lock_guard<std::mutex> lock(mtx);
    clients.erase(std::remove(clients.begin(), clients.end(), client),clients.end());
}

void loadMessageQueue(int client){
    std::lock_guard<std::mutex> lock(mtx);
    for(std::string msg : messageQueue)
        send(client, msg.c_str(), msg.size(),0);
}


int main(){
    int server = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in socketAddress;
    socketAddress.sin_family=AF_INET;
    socketAddress.sin_port=htons(8080);
    socketAddress.sin_addr.s_addr=INADDR_ANY;

    if(::bind(server, (struct sockaddr*) &socketAddress, sizeof(socketAddress))<0){
        std::cout<<"Error while binding the socket to the port";
    }else{
        std::cout<<"Binding Successful!"<<std::endl;
    }

    listen(server,10);

    std::cout<<"Server running on port : "<<ntohs(socketAddress.sin_port)<<"..."<<std::endl;

    while(true){
        int client=accept(server,nullptr,nullptr);
        {
            std::lock_guard<std::mutex> lock(mtx);
            clients.push_back(client);
        }
        loadMessageQueue(client);

        std::thread(handleClient, client).detach();
    }
}