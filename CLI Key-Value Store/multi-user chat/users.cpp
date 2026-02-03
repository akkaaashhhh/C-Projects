#include<iostream>
#include<thread>
#include<cstring>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>

void receiveLoop(int sock){
    char buffer[1024];

    while(true){
        int n = recv(sock, buffer, sizeof(buffer)-1,0);
        if(n<=0) break;

        buffer[n]='\0';
        std::cout<<buffer;
    }
}


int main(){
    int endpoint = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in endpointAddress;
    endpointAddress.sin_family=AF_INET;
    endpointAddress.sin_port=htons(8080);
    endpointAddress.sin_addr.s_addr=inet_addr("127.0.0.1");

    connect(endpoint, (sockaddr*) &endpointAddress, sizeof(endpointAddress));

    std::thread t(receiveLoop, endpoint);
    std::string message;

    while(std::getline(std::cin, message)){
        send(endpoint,message.c_str(), message.size(), 0);
    }

    close(endpoint);
    t.join();
}