#include "dict.hpp"

#include "utils.hpp"

#include <algorithm>
#include <cstring>
#include <functional>

DictRecord::DictRecord() {}

DictRecord::DictRecord(uint32_t postings_id, uint32_t str_offset):
    postings_id(postings_id), str_offset(str_offset) {}

void Dict::Read(std::ifstream& fin) {
    ReadBinaryVector(records, fin);
    ReadBinaryVector(strings, fin);
}

void Dict::Write(std::ofstream& fout) const {
    WriteBinaryVector(records, fout);
    WriteBinaryVector(strings, fout);
}

void Dict::Add(const std::string& word, uint32_t postings_id) {
    records.push_back(DictRecord(postings_id, strings.size()));
    for (size_t i = 0;i < word.length();i++) {
        strings.push_back(word[i]);
    }
    strings.push_back('\0');
}

void Dict::Find(const std::unordered_set<std::string>& words, std::vector<uint32_t>& postings_ids) const {
    postings_ids.clear();
    for (const std::string& word : words) {
        uint32_t postings_id = 0;
        if (Find(word, postings_id)) {
            postings_ids.push_back(postings_id);
        }
    }
}

bool Dict::Find(const std::string& word, uint32_t& postings_id) const {
    using namespace std::placeholders;
    auto it = std::lower_bound(records.begin(), records.end(), word, std::bind(&Dict::IsLess, this, _1, _2));
    if (it != records.end() && IsEqual(*it, word)) {
        postings_id = it->postings_id;
        return true;
    }
    return false;
}

bool Dict::IsLess(const DictRecord& record, const std::string& word) const {
    return strcmp(&strings[record.str_offset], word.c_str()) < 0;
}

bool Dict::IsEqual(const DictRecord& record, const std::string& word) const {
    return strcmp(&strings[record.str_offset], word.c_str()) == 0;
}
