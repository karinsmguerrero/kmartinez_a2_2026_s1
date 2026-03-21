#ifndef SERIAL_H
#define SERIAL_H

#include <iostream>
#include <string>
#include <algorithm>
#include "../FileManagement/FileReader.h"
#include "../Mapper/Mapper.h"
#include "Stalls.h"
#include "../Utils/TopWords.h"

class Serial
{
public:
    int runMapReduce(std::string filePath, std::unordered_map<std::string, int> &localHashMap, int seed);
};

#endif // SERIAL_H