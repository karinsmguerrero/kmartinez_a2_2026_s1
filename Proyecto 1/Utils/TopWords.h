#ifndef TOP_WORDS_H
#define TOP_WORDS_H

#include <string>
#include <algorithm>
#include <vector>
#include <unordered_map>
#include <iostream>

std::vector<std::string> getTopKWords(const std::unordered_map<std::string, int> &wordCounts, size_t k);
void printTopKWordCounts(std::vector<std::string> topKWords, const std::unordered_map<std::string, int> &hashMap);

#endif // TOP_WORDS_H