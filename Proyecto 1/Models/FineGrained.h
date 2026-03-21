#ifndef FINEGRAINED_H
#define FINEGRAINED_H

#include <iostream>
#include <string>
#include <algorithm>
#include <pthread.h>
#include "../FileManagement/FileReader.h"
#include "../Mapper/Mapper.h"
#include "Stalls.h"

class FineGrained
{
private:
    static int global_clock;
    static int threads_completed; // Tracks when all work is done
    static int total_threads;     // Total number of threads to be used

    static pthread_mutex_t pipeline_mutex;
    static pthread_cond_t clock_tick;

public:
    struct ThreadData
    {
        int thread_id;
        std::vector<std::string> text_chunk;
        int chunk_size;
        std::unordered_map<std::string, int> localHashMap;
        int seed;
    };

    static void *map(void *arg);
    int runMapReduce(std::string filePath, int numThreads, std::unordered_map<std::string, int> &globalHashMap, int seed);
};

#endif