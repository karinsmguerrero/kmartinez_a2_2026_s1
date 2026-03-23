#ifndef SMT_H
#define SMT_H

#include <iostream>
#include <string>
#include <pthread.h>
#include "../Mapper/Mapper.h"
#include "../Reducer/Reducer.h"
#include "ThreadData.h"
#include "Stalls.h"

class SMT
{
public:

    static void *map(void *arg);
    ThreadResults runMapReduce(std::vector<std::string> lines, int numThreads, std::unordered_map<std::string, int> &globalHashMap);
};

#endif // SMT_H