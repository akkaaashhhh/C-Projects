#include "Server.h"

#include <iostream>
#include <thread>
#include <algorithm>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>



// ------------------------------------------------
// Constructor
// ------------------------------------------------
Server::Server(int portNumber)
    : port(portNumber), nextId(1)
{
    listenSock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (::bind(listenSock, (sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(1);
    }

    listen(listenSock, 10);

    std::cout << "Server listening on port " << port << "\n";
}



// ------------------------------------------------
// run()
// main accept loop
// ------------------------------------------------
void Server::run()
{
    while (true)
    {
        acceptClient();
    }
}



// ------------------------------------------------
// acceptClient()
// ------------------------------------------------
void Server::acceptClient()
{
    int clientSock = accept(listenSock, nullptr, nullptr);
    if (clientSock < 0)
        return;

    Client client(clientSock, nextId++);

    {
        std::lock_guard<std::mutex> lock(mtx);
        clients.push_back(client);
    }

    std::cout << "Client " << client.getId() << " connected\n";

    std::thread(&Server::handleClient, this, client).detach();
}



// ------------------------------------------------
// handleClient()
// thread per client (for now)
// ------------------------------------------------
void Server::handleClient(Client client)
{
    std::string message;

    while (true)
    {
        if (!client.receive(message))
            break;

        std::string formatted =
            "Client " + std::to_string(client.getId()) +
            ": " + message + "\n";

        std::cout << formatted;

        broadcast(formatted, client.getId());
    }

    client.close();
    removeClient(client.getId());

    std::cout << "Client " << client.getId() << " disconnected\n";
}



// ------------------------------------------------
// broadcast()
// ------------------------------------------------
void Server::broadcast(const std::string& msg, int senderId)
{
    std::lock_guard<std::mutex> lock(mtx);

    for (auto& c : clients)
    {
        if (c.getId() != senderId)
            c.send(msg);
    }
}



// ------------------------------------------------
// removeClient()
// ------------------------------------------------
void Server::removeClient(int id)
{
    std::lock_guard<std::mutex> lock(mtx);

    clients.erase(
        std::remove_if(clients.begin(), clients.end(),
            [&](const Client& c)
            {
                return c.getId() == id;
            }),
        clients.end());
}