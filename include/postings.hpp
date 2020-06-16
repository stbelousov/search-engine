#pragma once

#include <fstream>
#include <vector>

struct Posting {
    uint32_t doc;
    float rank;

public:
    Posting();
    Posting(uint32_t doc, float rank);

    bool operator!=(const Posting& p) const;
    bool operator==(const Posting& p) const;
    bool operator<(const Posting& p) const;

    static bool CompareByRank(const Posting& a, const Posting& b);
};

class Postings {
public:
    void Read(std::ifstream& fin);
    void Write(std::ofstream& fout) const;

    uint32_t Add(uint32_t doc, float rank);
    void AddDelimiter();

    void Intersect(const std::vector<uint32_t>& postings_ids, std::vector<Posting>& result) const;

    bool IsEmpty() const;

private:
    std::vector<Posting> postings;
};
