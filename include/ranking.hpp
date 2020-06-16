#pragma once

#include <cstdint>

float BM25(double idf, uint32_t freq, uint32_t doc_len, double avg_doc_len);
