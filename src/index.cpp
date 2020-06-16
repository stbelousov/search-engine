#include "index.hpp"

#include "utils.hpp"

#include <algorithm>
#include <cinttypes>
#include <iostream>
#include <fstream>

Index::Index(uint64_t min_doc): header(min_doc) {}

Index::Index(const std::string& fname) {
    std::ifstream fin(fname, std::ios::in | std::ios::binary);
    if (!fin) {
        throw std::runtime_error("Cannot open the index file");
    }
    header.Read(fin);
    dict.Read(fin);
    postings.Read(fin);
}

void Index::Add(const std::string& word, uint64_t doc, float rank, bool is_new_word) {
    if (is_new_word && !postings.IsEmpty()) {
        postings.AddDelimiter();
    }
    uint32_t postings_id = postings.Add(doc - header.min_doc, rank);
    if (is_new_word) {
        dict.Add(word, postings_id);
    }
}

void Index::Finalize() {
    postings.AddDelimiter();
}

void Index::Write(const std::string& fname) const {
    std::ofstream fout(fname, std::ios::out | std::ios::binary);
    if (!fout) {
        throw std::runtime_error("Cannot open the index file");
    }
    header.Write(fout);
    dict.Write(fout);
    postings.Write(fout);
}

void Index::ProcessQuery(const std::string& query) const {
    std::unordered_set<std::string> words;
    split_query(normalize_query(query), words);

    std::vector<uint32_t> postings_ids;
    dict.Find(words, postings_ids);

    // Nothing is found
    if (words.empty() || words.size() != postings_ids.size()) {
        return;
    }

    std::vector<Posting> result;
    postings.Intersect(postings_ids, result);

    std::sort(result.begin(), result.end(), Posting::CompareByRank);
    for (const Posting& p : result) {
        uint64_t doc = header.min_doc + p.doc;
        printf("WSJ%" PRIu64 "-%04" PRIu64 " %.8f\n", doc / kDocnoDelimiter, doc % kDocnoDelimiter, p.rank);
    }
}
