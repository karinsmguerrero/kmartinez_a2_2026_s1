#include <iostream>
#include <string>
#include <algorithm> // for std::min
#include "FileManagement/FileReader.h"

int main() {
    // Functional approach: Read file and get lines
    auto lines = readFileToLines("Assets/output.txt");

    if (!lines.empty()) {
        std::cout << "File read successfully. Number of lines: " << getLineCount(lines) << std::endl;

        // Print first 5 lines
        size_t numToPrint = std::min(size_t(5), getLineCount(lines));
        for (size_t i = 0; i < numToPrint; ++i) {
            std::cout << "Line " << i + 1 << ": " << getLine(lines, i) << std::endl;
        }
    } else {
        std::cout << "Failed to read file." << std::endl;
    }

    return 0;
}