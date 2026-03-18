#include <iostream>
#include <string>
#include <algorithm>
#include "Models/Serial.cpp"

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
    // List of files to process
    std::vector<std::string> files = {
        "Assets/conde.txt",
        "Assets/fellowship.txt",
        "Assets/In Search of Lost Time.txt"};

    std::cout << "Select a model to run:" << std::endl;
    std::cout << "1. Serial Model" << std::endl;
    std::cout << "2. Parallel Model" << std::endl;
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
        std::cout << "------------ Running Serial Model on " << files[fileChoice - 1] << "------------ "<< std::endl;
        runMapReduce(files[fileChoice - 1]); // Process the selected file
        printTopKWordCounts(getTopKWords(localHashMap, 10), localHashMap); // Print the top 10 most common words
    }
    else if (modelChoice == 2)
    {
        std::cout << "------------ Running Parallel Model on " << files[fileChoice - 1] << "------------ "<< std::endl;
    }
    else
    {
        std::cout << "Invalid choice. Exiting." << std::endl;
        return 1;
    }

    return 0;
}

// TO DO
/*
1. Take text vector and divide it into smaller vectors of a specified size (e.g., 100 lines each).
2. Create a thread for each smaller vector to process it concurrently.
3. Each thread will perform the following tasks:
    a. Count the occurrences of each word in its assigned vector.
    b. Store the word counts in a local data structure (e.g., a map or dictionary).
4. After all threads have completed their processing, the main thread will:
    a. Collect the word counts from each thread.
    b. Combine the counts to get the total occurrences of each word across all vectors.
5. Finally, the program will output the total word counts.
*/

/*
size_t chunkSize = 100; // Number of lines per chunk
size_t totalLines = getLineCount(lines);
size_t numChunks = (totalLines + chunkSize - 1) / chunkSize; // Calculate number of chunks needed

for (size_t i = 0; i < numChunks; ++i) {
    size_t start = i * chunkSize;
    size_t end = std::min(start + chunkSize, totalLines);
    std::vector<std::string> textChunk(lines.begin() + start, lines.begin() + end);
    processTextChunk(textChunk); // Process the chunk of text
}*/