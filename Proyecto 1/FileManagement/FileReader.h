#ifndef FILEREADER_H
#define FILEREADER_H

#include <fstream>
#include <vector>
#include <string>
#include <iostream>

std::vector<std::string> readFileToLines(const std::string& filename);
size_t getLineCount(const std::vector<std::string>& lines);
std::string getLine(const std::vector<std::string>& lines, size_t index);   
#endif // FILEREADER_H