#include <iostream>
#include <string>
#include <algorithm>
#include "Models/Serial.cpp"
#include "Models/FineGrained.cpp"

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
        std::cout << word << ": " << hashMap.at(word) << std::endl;
    }
}

int main()
{
    std::vector<std::string> files = {
        "Assets/conde.txt",
        "Assets/fellowship.txt",
        "Assets/In Search of Lost Time.txt"};

    std::cout << "Select a model to run:" << std::endl;
    std::cout << "1. Serial Model" << std::endl;
    std::cout << "2. Parallel Fine Grained Model" << std::endl;
    std::cout << "Enter your choice (1 or 2): ";
    int modelChoice;
    std::cin >> modelChoice;

    std::cout << "Select text to process: " << std::endl;
    std::cout << "1. El Conde de Monte Cristo" << std::endl;
    std::cout << "2. The Fellowship of the Ring" << std::endl;
    std::cout << "3. In Search of Lost Time" << std::endl;
    std::cout << "Enter your choice (1, 2, or 3): ";
    int fileChoice;
    std::cin >> fileChoice;

    if (modelChoice == 1)
    {
        std::cout << "------------ Running Serial Model on " << files[fileChoice - 1] << "------------ " << std::endl;
        std::unordered_map<std::string, int> globalHashMap;
        runMapReduce_serial(files[fileChoice - 1], globalHashMap);           
        printTopKWordCounts(getTopKWords(globalHashMap, 10), globalHashMap); 
    }
    else if (modelChoice == 2)
    {
        std::cout << "------------ Running Parallel Fine Grained Model on " << files[fileChoice - 1] << "------------ " << std::endl;
        std::unordered_map<std::string, int> globalHashMap;         
        runMapReduce_fine(files[fileChoice - 1], 4, globalHashMap);
        //std::cout << "Global hash map size: " << globalHashMap.size() << std::endl;
        std::cout << "Top 10 most common words in " << files[fileChoice - 1] << ":" << std::endl;
        printTopKWordCounts(getTopKWords(globalHashMap, 10), globalHashMap); 
    }
    else
    {
        std::cout << "Invalid choice. Exiting." << std::endl;
        return 1;
    }

    return 0;
}
