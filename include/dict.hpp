#pragma once

#include <fstream>
#include <vector>
#include <unordered_set>

struct DictRecord {
    uint32_t postings_id = 0;
    uint32_t str_offset = 0;

public:
    DictRecord();
    DictRecord(uint32_t postings_id, uint32_t str_offset);
};

class Dict {
public:
    void Read(std::ifstream& fin);
    void Write(std::ofstream& fout) const;

    void Add(const std::string& word, uint32_t postings_id);

    void Find(const std::unordered_set<std::string>& words, std::vector<uint32_t>& postings_ids) const;

private:
    bool Find(const std::string& word, uint32_t& postings_id) const;
    bool IsLess(const DictRecord& record, const std::string& word) const;
    bool IsEqual(const DictRecord& record, const std::string& word) const;

private:
    std::vector<DictRecord> records;
    std::vector<char> strings;
};
