#pragma once
#include <string>
#include <unordered_map>
#include <optional>

class Storage {
private:
    std::unordered_map<std::string, std::string> data;

public:
    void set(const std::string& key, const std::string& value);
    std::optional<std::string> get(const std::string& key) const;
    void remove(const std::string& key);

    const std::unordered_map<std::string, std::string>& raw() const;
    void loadRaw(const std::unordered_map<std::string, std::string>& d);
};