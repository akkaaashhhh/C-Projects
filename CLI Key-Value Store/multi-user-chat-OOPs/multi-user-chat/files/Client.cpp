#include "Client.h"
#include<sys/socket.h>
#include<unistd.h>
#include<cstring>


Client::Client(int socketFd, int clientId) : sock(socketFd, id(clientId)){}

int Client::getId() const{
    return id;
}

int Client::getSocket() const{
    return sock;
}

bool Client::receive(std::string& outMessage){
    char buffer[1024];

    int n = recv(sock, buffer, sizeof(buffer)-1,0);

    if(n<=0) return false;

    buffer[n]='\0';

    outMessage=buffer;

    return true;
}
void Client::send(const std::string& message){
    ::send(sock, message.c_str(), message.size(),0);
}

void Client::close(){
    if(sock>=0){
        ::close(sock);
        sock=-1;
    }
}