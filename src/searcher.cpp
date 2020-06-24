#include "index.hpp"
#include "utils.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <index_file>" << std::endl;
        return 1;
    }

    // Read the index
    std::string index_file = argv[1];
    Index index(index_file);

    // Process queries
    std::string query;
    while (std::getline(std::cin, query)) {
        index.ProcessQuery(query);
    }

    return 0;
}
