#include "Storage.h"

void Storage::set(const std::string& key, const std::string& value) {
    data[key] = value;
}

std::optional<std::string> Storage::get(const std::string& key) const {
    auto it = data.find(key);
    if (it == data.end()) return std::nullopt;
    return it->second;
}

void Storage::remove(const std::string& key) {
    data.erase(key);
}

const std::unordered_map<std::string, std::string>& Storage::raw() const {
    return data;
}

void Storage::loadRaw(const std::unordered_map<std::string, std::string>& d) {
    data = d;
}