#ifndef SMT_H
#define SMT_H

#include <iostream>
#include <string>
#include <pthread.h>
#include "../Mapper/Mapper.h"
#include "Stalls.h"

class SMT
{
public:
    struct ThreadData
    {
        int thread_id;
        std::vector<std::string> text_chunk;
        int chunk_size;
        std::unordered_map<std::string, int> localHashMap;
        int seed;
    };

    struct ThreadResults
    {
        int clock_ticks;
        int total_stalls;
    };

    static ThreadResults *map(void *arg);
    ThreadResults runMapReduce(std::vector<std::string> lines, int numThreads, std::unordered_map<std::string, int> &globalHashMap, int seed);
};

#endif // SMT_H