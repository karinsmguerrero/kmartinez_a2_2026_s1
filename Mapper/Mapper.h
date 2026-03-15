#ifndef MAPPER_H
#define MAPPER_H

#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <cctype>
#include <algorithm>

extern std::vector<std::string> textChunk; // Vector to hold a chunk of text for processing
extern std::unordered_map<std::string, int> localHashMap; // Local hash map for counting words in a chunk

void processTextChunk(const std::vector<std::string>& chunk); 
void printWordCounts(); 
std::string removePunctuation(const std::string& word); 

#endif // MAPPER_H