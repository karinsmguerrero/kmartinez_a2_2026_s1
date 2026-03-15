#include <iostream>
#include <string>
#include <algorithm> // for std::min
#include "FileManagement/FileReader.h"
#include "Mapper/Mapper.h"

int main() {
    // Read file and get lines
    auto lines = readFileToLines("Assets/conde.txt");

    if (!lines.empty()) {
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

        size_t chunkSize = 100; // Number of lines per chunk
        size_t totalLines = getLineCount(lines);
        size_t numChunks = (totalLines + chunkSize - 1) / chunkSize; // Calculate number of chunks needed

        for (size_t i = 0; i < numChunks; ++i) {
            size_t start = i * chunkSize;
            size_t end = std::min(start + chunkSize, totalLines);
            std::vector<std::string> textChunk(lines.begin() + start, lines.begin() + end);
            processTextChunk(textChunk); // Process the chunk of text
        }
        printWordCounts(); // Print the final word counts
        
    } else {
        std::cout << "Failed to read file." << std::endl;
    }

    return 0;
}