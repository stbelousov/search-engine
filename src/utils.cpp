#include "utils.hpp"

#include <cstring>
#include <iomanip>
#include <iostream>
#include <regex>
#include <sstream>

// Regexps for query normalization, the same as in parser.py
static const std::regex skip_punctuation_re("[^\\sa-z0-9#$%'&+/=.:<>]+");
static const std::regex skip_special_re("[.:]+([^0-9]|$)");

uint64_t DocnoToLong(const std::string& docno) {
    uint32_t first = 0, second = 0;
    sscanf(docno.c_str(), "WSJ%u-%u", &first, &second);
    return first * kDocnoDelimiter + second;
}

std::string normalize_query(const std::string& query) {
    std::string normalized = query;
    std::transform(normalized.begin(), normalized.end(), normalized.begin(),
        [](unsigned char c){ return std::tolower(c); });
    std::replace(normalized.begin(), normalized.end(), '-', ' ');
    normalized = std::regex_replace(normalized, skip_punctuation_re, "");
    normalized = std::regex_replace(normalized, skip_special_re, "$1");
    return normalized;
}

void split_query(const std::string& query, std::unordered_set<std::string>& words) {
    words.clear();
    std::istringstream iss(query);
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(),
              std::inserter(words, words.begin()));
}
