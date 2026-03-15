#include "Mapper.h"

// Helper function to check if a character is punctuation (including Spanish punctuation)
bool isPunctuation(char c) {
    // Standard punctuation
    if (::ispunct(static_cast<unsigned char>(c))) {
        return true;
    }
    // Spanish-specific punctuation (check byte values for UTF-8)
    // ¡ is 0xC2 0xA1 in UTF-8, ¿ is 0xC2 0xBF in UTF-8
    // But since we're dealing with char, we need to handle this differently
    return false; // For now, rely on standard punctuation
}

// Function to remove all punctuation from a word
std::string removePunctuation(const std::string& word) {
    std::string result;
    std::string punctuation = ".,;:!?\"'()[]{}¡¿"; // Include Spanish punctuation
    
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
            // Remove punctuation marks from the word (including Spanish punctuation)
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