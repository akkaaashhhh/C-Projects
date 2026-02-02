#include "Persistence.h"
#include <fstream>

Persistence::Persistence(std::string file)
    : filename(std::move(file)) {}

void Persistence::save(const std::unordered_map<std::string, std::string>& data) {
    std::ofstream out(filename);

    for (auto& [k, v] : data) {
        out << k << ":" << v << "\n";
    }
}

std::unordered_map<std::string, std::string> Persistence::load() {
    std::unordered_map<std::string, std::string> result;

    std::ifstream in(filename);
    if (!in) return result;

    std::string line;

    while (std::getline(in, line)) {
        auto pos = line.find(':');
        if (pos == std::string::npos) continue;

        auto key = line.substr(0, pos);
        auto val = line.substr(pos + 1);

        result[key] = val;
    }

    return result;
}