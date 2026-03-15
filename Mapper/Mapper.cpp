#include "Mapper.h"

// Function to remove all punctuation from a word
std::string removePunctuation(const std::string& word) {
    std::string result;
    std::string punctuation = ".,;:!?\"'()[]{}¡¿";
    
    for (char c : word) {
        if (punctuation.find(c) == std::string::npos) {
            result += c;
        }
    }
    return result;
}

void processTextChunk(const std::vector<std::string>& chunk) {
    // Function to process a chunk of text and count word occurrences
    for (const auto& line : chunk) {
        std::istringstream iss(line);
        std::string word;
        while (iss >> word) {
            // Remove punctuation marks from the word 
            word = removePunctuation(word);
            // Only count non-empty words after removing punctuation
            if (!word.empty()) {
                ++localHashMap[word]; // Increment the count for the word
            }
        }
    }
}

void printWordCounts() {
    // Function to print the word counts
    for (const auto& pair : localHashMap) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
}