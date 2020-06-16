#pragma once

#include <cstdint>
#include <fstream>

struct Header {
    uint64_t min_doc;

public:
    Header();
    explicit Header(uint64_t min_doc);

    void Read(std::ifstream& fin);
    void Write(std::ofstream& fout) const;
};
