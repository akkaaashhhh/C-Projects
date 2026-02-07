#include <iostream>
#include <thread>
#include <string>
#include <cstring>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

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
    int sock = socket(AF_INET,SOCK_STREAM,0);

    sockaddr_in server{};

    server.sin_family=AF_INET;
    server.sin_port=htons(8080);
    server.sin_addr.s_addr=inet_addr("127.0.0.1");

    if(connect(sock, (sockaddr*)&server, sizeof(server))<0){
        std::cout<<"Connection failed \n";
        return 1;
    }

    std::cout<< "Connected to that chat server\n";

    std::thread receiver(receiveLoop,sock);

    std::string msg;

    while(std::getline(std::cin, msg)){
        msg+= "\n";
        send(sock, msg.c_str(), msg.size(), 0);

        std::cout<<"You : "<<msg;
    }

    close(sock);
    receiver.join();

    return 0;
}