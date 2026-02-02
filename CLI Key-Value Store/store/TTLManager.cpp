#include "TTLManager.h"

void TTLManager::setTTL(const std::string& key, int seconds) {
    expiry[key] = Clock::now() + std::chrono::seconds(seconds);
}

bool TTLManager::expired(const std::string& key) {
    auto it = expiry.find(key);
    if (it == expiry.end()) return false;
    return Clock::now() >= it->second;
}

void TTLManager::remove(const std::string& key) {
    expiry.erase(key);
}