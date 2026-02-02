#include "CommandParser.h"
#include <sstream>
#include <iostream>

bool CommandParser::execute(const std::string& line, KeyValueStore& store) {
    std::stringstream ss(line);

    std::string cmd;
    ss >> cmd;

    if (cmd == "SET") {
        std::string key, value, ttlToken;
        ss >> key >> value >> ttlToken;

        int ttl = -1;

        if (ttlToken.rfind("TTL=", 0) == 0)
            ttl = std::stoi(ttlToken.substr(4));

        store.set(key, value, ttl);
    }
    else if (cmd == "GET") {
        std::string key;
        ss >> key;

        auto val = store.get(key);
        if (val)
            std::cout << *val << "\n";
        else
            std::cout << "NULL\n";
    }
    else if (cmd == "DELETE") {
        std::string key;
        ss >> key;
        store.remove(key);
    }
    else if (cmd == "SAVE") {
        store.save();
    }
    else if (cmd == "LOAD") {
        store.load();
    }
    else if (cmd == "EXIT") {
        return false;
    }

    return true;
}