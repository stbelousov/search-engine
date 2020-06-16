#pragma once

#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <unordered_set>

// A constant for splitting the DOCNO into two numbers (separated by '-')
static constexpr uint64_t kDocnoDelimiter = 10000;

template <typename T>
void ReadBinary(T& val, std::ifstream& fin) {
    fin.read(reinterpret_cast<char*>(&val), sizeof(val));
}

template <typename T>
void WriteBinary(const T& val, std::ofstream& fout) {
    fout.write(reinterpret_cast<const char*>(&val), sizeof(val));
}

template <typename T>
void ReadBinaryVector(std::vector<T>& v, std::ifstream& fin) {
    size_t len;
    ReadBinary(len, fin);
    v.resize(len);
    fin.read(reinterpret_cast<char*>(&v[0]), len * sizeof(T));
}

template <typename T>
void WriteBinaryVector(const std::vector<T>& v, std::ofstream& fout) {
    size_t len = v.size();
    WriteBinary(len, fout);
    fout.write(reinterpret_cast<const char*>(&v[0]), len * sizeof(T));
}

uint64_t DocnoToLong(const std::string& docno);

std::string normalize_query(const std::string& query);
void split_query(const std::string& query, std::unordered_set<std::string>& words);
