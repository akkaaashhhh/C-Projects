#include<cstring>
#include<iostream>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>

using namespace std;

int main(){

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0){
        perror("socket");
        return 1;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if(::bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
        perror("bind");
        return 1;
    }

    if(listen(serverSocket,5) < 0){
        perror("listen");
        return 1;
    }

    cout << "Server listening on "<< ntohs(serverAddress.sin_port) <<"...\n";

    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if(clientSocket < 0){
        perror("accept");
        return 1;
    }

    char buffer[1024] = {0};

    int n = recv(clientSocket, buffer, sizeof(buffer),0);
    if(n < 0){
        perror("recv");
        return 1;
    }

    cout<<"The message from the client is : "<<buffer<<endl;

    return 0;
}