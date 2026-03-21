#include <iostream>

#include "Models/Serial.h"
#include "Models/FineGrained.h"
#include "Models/CoarseGrained.h"
#include "Utils/TopWords.h"

int main()
{
    std::string files[] = {
        "Assets/conde.txt",
        "Assets/fellowship.txt",
        "Assets/In Search of Lost Time.txt"};

    std::cout << "Select a model to run:" << std::endl;
    std::cout << "1. Serial Model" << std::endl;
    std::cout << "2. Parallel Fine Grained Model" << std::endl;
    std::cout << "3. Parallel Coarse Grained Model" << std::endl;
    std::cout << "Enter your choice (1, 2, or 3): ";
    int modelChoice;
    std::cin >> modelChoice;

    std::cout << "Select text to process: " << std::endl;
    std::cout << "1. El Conde de Monte Cristo" << std::endl;
    std::cout << "2. The Fellowship of the Ring" << std::endl;
    std::cout << "3. In Search of Lost Time" << std::endl;
    std::cout << "Enter your choice (1, 2, or 3): ";
    int fileChoice;
    std::cin >> fileChoice;

    int seed = clock() + std::hash<std::thread::id>()(std::this_thread::get_id()); // Seed for random number generator based on current time and thread ID

    if (modelChoice == 1)
    {
        std::cout << "------------ Running Serial Model on " << files[fileChoice - 1] << "------------ " << std::endl;
        std::unordered_map<std::string, int> globalHashMap;

        Serial serialModel;
        serialModel.runMapReduce(files[fileChoice - 1], globalHashMap, seed);
        printf("Top 10 most common words in %s:\n", files[fileChoice - 1].c_str());
        printTopKWordCounts(getTopKWords(globalHashMap, 10), globalHashMap);
    }
    else if (modelChoice == 2)
    {
        std::cout << "------------ Running Parallel Fine Grained Model on " << files[fileChoice - 1] << "------------ " << std::endl;
        std::unordered_map<std::string, int> globalHashMap;

        FineGrained fineGrainedModel;
        fineGrainedModel.runMapReduce(files[fileChoice - 1], 4, globalHashMap, seed);
        printf("Top 10 most common words in %s:\n", files[fileChoice - 1].c_str());
        printTopKWordCounts(getTopKWords(globalHashMap, 10), globalHashMap);
    }
    else if (modelChoice == 3)
    {
        std::cout << "------------ Running Parallel Coarse Grained Model on " << files[fileChoice - 1] << "------------ " << std::endl;
        std::unordered_map<std::string, int> globalHashMap;

        CoarseGrained coarseGrainedModel;
        coarseGrainedModel.runMapReduce(files[fileChoice - 1], 4, globalHashMap, seed);

        printf("Top 10 most common words in %s:\n", files[fileChoice - 1].c_str());
        printTopKWordCounts(getTopKWords(globalHashMap, 10), globalHashMap);
    }
    else
    {
        std::cout << "Invalid choice. Exiting." << std::endl;
        return 1;
    }

    return 0;
}
