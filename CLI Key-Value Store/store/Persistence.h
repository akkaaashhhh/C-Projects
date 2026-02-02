#pragma once
#include <unordered_map>
#include <string>

class Persistence {
private:
    std::string filename;

public:
    explicit Persistence(std::string file = "dump.db");

    void save(const std::unordered_map<std::string, std::string>& data);
    std::unordered_map<std::string, std::string> load();
};