#include "header.hpp"

#include "utils.hpp"

Header::Header(): min_doc(0) {}

Header::Header(uint64_t min_doc): min_doc(min_doc) {}

void Header::Read(std::ifstream& fin) {
    ReadBinary(min_doc, fin);
}

void Header::Write(std::ofstream& fout) const {
    WriteBinary(min_doc, fout);
}
