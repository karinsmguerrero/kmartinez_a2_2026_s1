#include <iostream>
#include <string>
#include "FileManagement/FileReader.cpp"

int main() {
    FileReader reader;

    // Test reading from a file
    if (reader.readFile("Assets/output.txt")) {
        std::cout << "File read successfully. Number of lines: " << reader.getLineCount() << std::endl;

        // Print first 5 lines
        for (size_t i = 0; i < std::min(size_t(5), reader.getLineCount()); ++i) {
            std::cout << "Line " << i + 1 << ": " << reader.getLine(i) << std::endl;
        }
    } else {
        std::cout << "Failed to read file." << std::endl;
    }

    return 0;
}