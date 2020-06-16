#include "ranking.hpp"

#include <cmath>

// Okapi BM25 ranking function
float BM25(double idf, uint32_t freq, uint32_t doc_len, double avg_doc_len) {
    const double k1 = 1.2;
    const double b = 0.75;
    double K = k1 * ((1 - b) + (b * doc_len) / avg_doc_len);
    return idf * (freq * (k1 + 1)) / (freq + K);
}
