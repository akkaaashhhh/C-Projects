#include "store/KeyValueStore.h"
#include "cli/CommandParser.h"
#include <iostream>

int main() {
    KeyValueStore store;
    CommandParser parser;

    std::cout << "KV Store started. Commands: SET GET DELETE SAVE LOAD EXIT\n";

    std::string line;

    while (true) {
        std::getline(std::cin, line);

        if (!parser.execute(line, store))
            break;
    }

    return 0;
}