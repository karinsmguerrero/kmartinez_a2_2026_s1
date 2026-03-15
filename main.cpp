#include <iostream>
#include <string>
#include <algorithm>
#include "FileManagement/FileReader.h"
#include "Mapper/Mapper.h"

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

int main()
{
    // List of files to process
    std::vector<std::string> files = {
        "Assets/conde.txt",
        "Assets/fellowship.txt",
        "Assets/In Search of Lost Time.txt"};

    // Read file and get lines
    std::vector<std::string> lines = readFileToLines(files[2]); // Read the first file for testing

    if (!lines.empty())
    {
        std::cout << "File read successfully. Number of lines: " << getLineCount(lines) << std::endl;

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

        processTextChunk(lines); // Process the chunk of text
        // printWordCounts(); // Print the final word counts
        std::vector<std::string> topkwords = getTopKWords(localHashMap, 10); // Get the top 10 most common words
        printWordCounts(topkwords);                                          // Print the top 10 most common words
    }
    else
    {
        std::cout << "Failed to read file." << std::endl;
    }

    return 0;
}