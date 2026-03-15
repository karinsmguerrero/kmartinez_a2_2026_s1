#include <iostream>
#include <string>
#include <algorithm>
#include "../FileManagement/FileReader.h"
#include "../Mapper/Mapper.h"

int runMapReduce(std::string filePath)
{
    // Read file and get lines
    std::vector<std::string> lines = readFileToLines(filePath);

    if (!lines.empty())
    {
        std::cout << "File read successfully. Number of lines: " << getLineCount(lines) << std::endl;
        // Map phase: Process the chunk of text and count word occurrences
        processTextChunk(lines);    
        // Reduce phase: Add the word counts
        // No Reduce phase needed for the serial model since we are directly updating the localHashMap in the Mapper
        // and there are no other threads to combine results with.
    }
    else
    {
        std::cout << "Failed to read file." << std::endl;
    }

    return 0;
}