#include "TopWords.h"

std::vector<std::string> getTopKWords(const std::unordered_map<std::string, int> &wordCounts, size_t k)
{
    // Create a vector of pairs from the unordered_map
    std::vector<std::pair<std::string, int>> wordCountVector(wordCounts.begin(), wordCounts.end());

    // Sort the vector based on the counts in descending order
    std::sort(wordCountVector.begin(), wordCountVector.end(),
              [](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b)
              {
                  return b.second < a.second;
              });

    // Extract the top k words
    std::vector<std::string> topKWords;
    for (size_t i = 0; i < std::min(k, wordCountVector.size()); ++i)
    {
        topKWords.push_back(wordCountVector[i].first);
    }

    return topKWords;
}

void printTopKWordCounts(std::vector<std::string> topKWords, const std::unordered_map<std::string, int> &hashMap)
{
    // Function to print the word counts
    for (const auto &word : topKWords)
    {
        printf("%s: %d\n", word.c_str(), hashMap.at(word));
    }
}