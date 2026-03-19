#ifndef MAPPER_H
#define MAPPER_H

#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <cctype>
#include <algorithm>

void processTextChunk(const std::vector<std::string> &chunk, std::unordered_map<std::string, int> &localHashMap);
void printHashMap();
std::string removePunctuation(const std::string &word);

#endif // MAPPER_H