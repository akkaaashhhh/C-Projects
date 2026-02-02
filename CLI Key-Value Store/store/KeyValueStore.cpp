#include "KeyValueStore.h"

void KeyValueStore::set(const std::string& key,
                        const std::string& value,
                        int ttlSeconds) {
    std::lock_guard<std::mutex> lock(mtx);

    storage.set(key, value);

    if (ttlSeconds > 0)
        ttl.setTTL(key, ttlSeconds);
}

std::optional<std::string> KeyValueStore::get(const std::string& key) {
    std::lock_guard<std::mutex> lock(mtx);

    if (ttl.expired(key)) {
        storage.remove(key);
        ttl.remove(key);
        return std::nullopt;
    }

    return storage.get(key);
}

void KeyValueStore::remove(const std::string& key) {
    std::lock_guard<std::mutex> lock(mtx);
    storage.remove(key);
    ttl.remove(key);
}

void KeyValueStore::save() {
    std::lock_guard<std::mutex> lock(mtx);
    persistence.save(storage.raw());
}

void KeyValueStore::load() {
    std::lock_guard<std::mutex> lock(mtx);
    storage.loadRaw(persistence.load());
}