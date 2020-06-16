#include "postings.hpp"

#include "utils.hpp"

// All the postings are stored in one vector, so we use a delimiter for postings of different words
static const Posting kPostingDelimiter = Posting(std::numeric_limits<uint32_t>::max(), 0);

// Speed up result construction by preallocating memory for it
static constexpr size_t kResultReservedSize = 250000;

Posting::Posting() {}

Posting::Posting(uint32_t doc, float rank): doc(doc), rank(rank) {}

bool Posting::operator!=(const Posting& p) const {
    return doc != p.doc;
}

bool Posting::operator==(const Posting& p) const {
    return doc == p.doc;
}

bool Posting::operator<(const Posting& p) const {
    return doc < p.doc;
}

bool Posting::CompareByRank(const Posting& a, const Posting& b) {
    return a.rank > b.rank;
}

void Postings::Read(std::ifstream& fin) {
    ReadBinaryVector(postings, fin);
}

void Postings::Write(std::ofstream& fout) const {
    WriteBinaryVector(postings, fout);
}

uint32_t Postings::Add(uint32_t doc, float rank) {
    postings.push_back(Posting(doc, rank));
    return postings.size() - 1;
}

void Postings::AddDelimiter() {
    postings.push_back(kPostingDelimiter);
}

void Postings::Intersect(const std::vector<uint32_t>& postings_ids, std::vector<Posting>& result) const {
    result.clear();
    result.reserve(kResultReservedSize);
    std::vector<uint32_t> ids = postings_ids;
    while (true) {
        bool has_intersection = true;
        float rank = 0.0f;
        for (uint32_t& id : ids) {
            // We use the first postings list (ids[0]) as a base for our intersection,
            // i.e. we compare the elements of all other lists with the elements of the first list
            while (postings[id] != kPostingDelimiter && postings[id] < postings[ids[0]]) {
                id++;
            }
            // If any of the lists ends, no more elements can be added to the intersection, so we can return
            if (postings[id] == kPostingDelimiter) {
                return;
            }
            // If the current list doesn't contain the current element from the first list,
            // this element cannot be in the intersection
            if (postings[ids[0]] < postings[id]) {
                has_intersection = false;
            }
            // BM25 is a sum of scores of (word, document) pairs. Here we compute this sum
            rank += postings[id].rank;
        }

        if (has_intersection) {
            // If all the current elements are equal, we can add it to the intersection and shift all the pointers
            result.emplace_back(postings[ids[0]].doc, rank);
            for (uint32_t& id : ids) {
                id++;
            }
        } else {
            // Otherwise, we can just shift the first list's pointer
            ids[0]++;
        }
    }
}

bool Postings::IsEmpty() const {
    return postings.empty();
}
