#ifndef SERIAL_H
#define SERIAL_H

#include <iostream>
#include <string>
#include "../Mapper/Mapper.h"
#include "Stalls.h"
#include "ThreadData.h"


class Serial
{
public:
    ThreadResults runMapReduce(std::vector<std::string> lines, std::unordered_map<std::string, int> &localHashMap, int seed);
};

#endif // SERIAL_H