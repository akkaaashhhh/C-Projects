#pragma once
#include<string>

class Client{
    private:
        int sock;
        int id;
    public:
        Client(int socketDf, int clientId);

        int getId() const;
        int getSocket() const;

        bool receive(std::string& outMessage);
        bool send(const std::string& message);

        void close();
};