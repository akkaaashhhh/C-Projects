#pragma once
#include "Storage.h"
#include "TTLManager.h"
#include "Persistence.h"
#include <mutex>

class KeyValueStore {
private:
    Storage storage;
    TTLManager ttl;
    Persistence persistence;

    std::mutex mtx;

public:
    void set(const std::string& key, const std::string& value, int ttlSeconds = -1);
    std::optional<std::string> get(const std::string& key);
    void remove(const std::string& key);

    void save();
    void load();
};