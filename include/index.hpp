#pragma once

#include "dict.hpp"
#include "header.hpp"
#include "postings.hpp"

class Index {
public:
    explicit Index(uint64_t min_doc);
    explicit Index(const std::string& fname);

    void Add(const std::string& word, uint64_t doc, float rank, bool is_new_word);
    void Finalize();

    void Write(const std::string& fname) const;

    void ProcessQuery(const std::string& query) const;

private:
    Header header;
    Dict dict;
    Postings postings;
};
