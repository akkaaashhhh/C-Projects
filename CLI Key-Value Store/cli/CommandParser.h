#pragma once
#include "../store/KeyValueStore.h"
#include <string>

class CommandParser {
public:
    bool execute(const std::string& line, KeyValueStore& store);
};