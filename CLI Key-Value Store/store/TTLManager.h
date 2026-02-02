#pragma once
#include <unordered_map>
#include <string>
#include <chrono>

class TTLManager {
private:
    using Clock = std::chrono::steady_clock;
    std::unordered_map<std::string, Clock::time_point> expiry;

public:
    void setTTL(const std::string& key, int seconds);
    bool expired(const std::string& key);
    void remove(const std::string& key);
};