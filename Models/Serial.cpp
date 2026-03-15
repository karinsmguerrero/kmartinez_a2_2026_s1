#include <iostream>
#include <string>
#include <algorithm>
#include "../FileManagement/FileReader.h"
#include "../Mapper/Mapper.h"

std::vector<std::string> getTopKWords(const std::unordered_map<std::string, int> &wordCounts, size_t k)
{
    // Create a vector of pairs from the unordered_map
    std::vector<std::pair<std::string, int>> wordCountVector(wordCounts.begin(), wordCounts.end());

    // Sort the vector based on the counts in descending order
    std::sort(wordCountVector.begin(), wordCountVector.end(),
              [](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b)
              {
                  return b.second < a.second; // Sort in descending order
              });

    // Extract the top k words
    std::vector<std::string> topKWords;
    for (size_t i = 0; i < std::min(k, wordCountVector.size()); ++i)
    {
        topKWords.push_back(wordCountVector[i].first);
    }

    return topKWords;
}

void printWordCounts(std::vector<std::string> topKWords)
{
    // Function to print the word counts
    for (const auto &word : topKWords)
    {
        std::cout << word << ": " << localHashMap[word] << std::endl;
    }
}

int runMapReduce(std::string filePath)
{
    // Read file and get lines
    std::vector<std::string> lines = readFileToLines(filePath);

    if (!lines.empty())
    {
        std::cout << "File read successfully. Number of lines: " << getLineCount(lines) << std::endl;

        processTextChunk(lines);                                             // Process the chunk of text
        std::vector<std::string> topkwords = getTopKWords(localHashMap, 10); // Get the top 10 most common words
        printWordCounts(topkwords);                                          // Print the top 10 most common words
    }
    else
    {
        std::cout << "Failed to read file." << std::endl;
    }

    return 0;
}