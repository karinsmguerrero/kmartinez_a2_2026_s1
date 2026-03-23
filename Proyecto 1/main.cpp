#include <iostream>

#include "Models/Serial.h"
#include "Models/FineGrained.h"
#include "Models/CoarseGrained.h"
#include "Models/SMT.h"
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
    std::cout << "4. SMT Model" << std::endl;
    std::cout << "Enter your choice: ";
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
        std::vector<std::string> lines = readFileToLines(files[fileChoice - 1]);
        if (!lines.empty())
        {
            Serial serialModel;
            serialModel.runMapReduce(lines, globalHashMap, seed);
            printf("Top 10 most common words in %s:\n", files[fileChoice - 1].c_str());
            printTopKWordCounts(getTopKWords(globalHashMap, 10), globalHashMap);
        }
        else
        {
            printf("Failed to read file.\n");
        }
    }
    else if (modelChoice == 2)
    {
        std::cout << "------------ Running Parallel Fine Grained Model on " << files[fileChoice - 1] << "------------ " << std::endl;
        std::unordered_map<std::string, int> globalHashMap;
        std::vector<std::string> lines = readFileToLines(files[fileChoice - 1]);
        if (!lines.empty())
        {
            FineGrained fineGrainedModel;
            fineGrainedModel.runMapReduce(lines, 4, globalHashMap, seed);
            printf("Top 10 most common words in %s:\n", files[fileChoice - 1].c_str());
            printTopKWordCounts(getTopKWords(globalHashMap, 10), globalHashMap);
        }
        else
        {
            printf("Failed to read file.\n");
        }
    }
    else if (modelChoice == 3)
    {
        std::cout << "------------ Running Parallel Coarse Grained Model on " << files[fileChoice - 1] << "------------ " << std::endl;
        std::unordered_map<std::string, int> globalHashMap;
        std::vector<std::string> lines = readFileToLines(files[fileChoice - 1]);

        CoarseGrained coarseGrainedModel;
        coarseGrainedModel.runMapReduce(lines, 4, globalHashMap, seed);

        printf("Top 10 most common words in %s:\n", files[fileChoice - 1].c_str());
        printTopKWordCounts(getTopKWords(globalHashMap, 10), globalHashMap);
    }
    else if (modelChoice == 4)
    {
        std::cout << "------------ Running SMT Model on " << files[fileChoice - 1] << "------------ " << std::endl;
        std::unordered_map<std::string, int> globalHashMap;
        std::vector<std::string> lines = readFileToLines(files[fileChoice - 1]);

        SMT smtModel;
        smtModel.runMapReduce(lines, 10, globalHashMap);

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
