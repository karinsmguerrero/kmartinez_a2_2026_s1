#include <fstream>
#include <vector>
#include <string>
#include <iostream>

class FileReader {
private:
    std::vector<std::string> lines;

public:
    FileReader() {}

    // Method to read from file and store lines in the array
    bool readFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }

        file.close();
        return true;
    }

    // Getter to access the stored lines
    const std::vector<std::string>& getLines() const {
        return lines;
    }

    // Method to get a specific line by index
    std::string getLine(size_t index) const {
        if (index < lines.size()) {
            return lines[index];
        }
        return "";
    }

    // Method to get the number of lines
    size_t getLineCount() const {
        return lines.size();
    }

    // Method to clear the stored lines
    void clear() {
        lines.clear();
    }
};