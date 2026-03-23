#ifndef COARSEGRAINED_H
#define COARSEGRAINED_H

#include <iostream>
#include <string>
#include <pthread.h>
#include "../FileManagement/FileReader.h"
#include "../Mapper/Mapper.h"
#include "../Reducer/Reducer.h"
#include "ThreadData.h"
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
    static void switch_to_next_thread();
    static ThreadResults *map(void *arg);
    ThreadResults runMapReduce(std::vector<std::string> lines, int numThreads, std::unordered_map<std::string, int> &globalHashMap, int seed);
};

#endif // COARSEGRAINED_H