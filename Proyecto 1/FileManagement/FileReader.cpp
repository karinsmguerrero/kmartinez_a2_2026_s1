#include "FileReader.h"

// Function to read file and return lines as vector
std::vector<std::string> readFileToLines(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return lines; // Return empty vector on error
    }

    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    file.close();
    return lines;
}

// Helper function to get line count 
size_t getLineCount(const std::vector<std::string>& lines) {
    return lines.size();
}

// Helper function to get a specific line
std::string getLine(const std::vector<std::string>& lines, size_t index) {
    if (index < lines.size()) {
        return lines[index];
    }
    return "";
}