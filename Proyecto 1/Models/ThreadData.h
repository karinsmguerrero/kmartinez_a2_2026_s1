#ifndef THREADDATA_H
#define THREADDATA_H

#include <vector>
#include <unordered_map>
#include <string>

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

#endif // THREADDATA_H