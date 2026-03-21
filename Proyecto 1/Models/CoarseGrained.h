#ifndef COARSEGRAINED_H
#define COARSEGRAINED_H

#include <iostream>
#include <string>
#include <algorithm>
#include <pthread.h>
#include "../FileManagement/FileReader.h"
#include "../Mapper/Mapper.h"
#include "Stalls.h"

class CoarseGrained
{
private:
    static int global_clock;
    static int threads_completed; // Tracks when all work is done
    static int total_threads;     // Total number of threads to be used

    static pthread_mutex_t pipeline_mutex;
    static pthread_cond_t clock_tick;

    // CGMT specific variables
    static int current_active_thread;
    static int *threads_done;

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

    static void switch_to_next_thread();
    static ThreadResults *map(void *arg);
    ThreadResults runMapReduce(std::string filePath, int numThreads, std::unordered_map<std::string, int> &globalHashMap, int seed);
};

#endif // COARSEGRAINED_H