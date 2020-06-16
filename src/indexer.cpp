#include "index.hpp"
#include "ranking.hpp"
#include "utils.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct WordDocPair {
    std::string word;
    uint64_t doc = 0;

    WordDocPair() {}
    WordDocPair(const std::string& word, uint64_t doc): word(word), doc(doc) {}

    bool operator<(const WordDocPair& p) const {
        if (word != p.word) return word < p.word;
        return doc < p.doc;
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <output_file>" << std::endl;
        return 1;
    }

    std::string output_file = argv[1];
    std::string line;
    bool is_new_doc = true;
    uint64_t cur_doc = 0;
    uint64_t min_doc = std::numeric_limits<uint64_t>::max();
    std::map<WordDocPair, uint32_t> word_doc_pairs;

    // Stats for ranking
    std::unordered_map<uint64_t, uint32_t> doc_lengths;
    std::unordered_map<std::string, uint32_t> docs_with_word;
    std::unordered_map<std::string, double> idf;
    std::unordered_set<std::string> cur_doc_words;
    uint32_t total_docs = 0, total_words = 0;

    // Read the input document by document
    while (std::getline(std::cin, line)) {
        if (line.empty()) {
            is_new_doc = true;
            continue;
        }

        if (is_new_doc) {
            cur_doc = DocnoToLong(line);
            min_doc = std::min(min_doc, cur_doc);
            cur_doc_words.clear();
            total_docs++;
            is_new_doc = false;
            continue;
        }

        const std::string& word = line;
        word_doc_pairs[WordDocPair(word, cur_doc)]++;

        // If a new element was inserted (i.e., we haven't encountered this word in this document before),
        // then we increment the number of documents containing this word
        if (cur_doc_words.insert(word).second) {
            docs_with_word[word]++;
        }

        doc_lengths[cur_doc]++;
        total_words++;
    }

    double avg_doc_len = double(total_words) / total_docs;

    // Compute IDFs
    for (const auto& it : docs_with_word) {
        const std::string& word = it.first;
        uint32_t num_docs = it.second;
        idf[word] = log((total_docs - num_docs + 0.5) / (num_docs + 0.5));
    }

    // Compute ranks and add word-doc pairs to the index
    Index index(min_doc);
    std::string prev_word;
    for (const auto& it : word_doc_pairs) {
        const std::string& word = it.first.word;
        uint64_t doc = it.first.doc;
        uint32_t freq = it.second;
        float rank = BM25(idf[word], freq, doc_lengths[doc], avg_doc_len);
        index.Add(word, doc, rank, prev_word != word);
        prev_word = word;
    }

    index.Finalize();
    index.Write(output_file);

    return 0;
}
