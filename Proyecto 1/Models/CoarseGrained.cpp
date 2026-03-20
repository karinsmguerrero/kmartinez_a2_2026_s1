#include <iostream>
#include <string>
#include <algorithm>
#include <pthread.h>
#include "../FileManagement/FileReader.h"
#include "../Mapper/Mapper.h"
#include "Stalls.h"

int global_clock = 0;
int threads_completed = 0; // Tracks when all work is done
int total_threads = 0;     // Total number of threads to be used

pthread_mutex_t pipeline_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t clock_tick = PTHREAD_COND_INITIALIZER;

// CGMT specific variables
int current_active_thread = 0;
int threads_completed = 0;
int *threads_done;

struct ThreadData
{
    int thread_id;
    std::vector<std::string> text_chunk;
    int chunk_size;
    std::unordered_map<std::string, int> localHashMap;
    int seed;
};

// Hardware scheduler: finds the next available thread
void switch_to_next_thread()
{
    int next_thread = (current_active_thread + 1) % total_threads;
    while (threads_done[next_thread] && threads_completed < total_threads)
    {
        next_thread = (next_thread + 1) % total_threads;
    }
    current_active_thread = next_thread;
}

void *map(void *arg)
{
    ThreadData *data = static_cast<ThreadData *>(arg);
    int tid = data->thread_id;
    int start_line = 0; // Local indices in text_chunk
    int end_line = data->text_chunk.size();
    int current_line = start_line;
    bool done = false;
    bool is_stalled = false;
    int total_stalls = 0;

    // Loop until ALL threads have finished their workloads
    while (1)
    {
        pthread_mutex_lock(&pipeline_mutex);

        // Check if everyone is done before waiting
        if (threads_completed == total_threads)
        {
            pthread_mutex_unlock(&pipeline_mutex);
            break;
        }
        // CGMT: Wait until the hardware schedules THIS thread
        while (current_active_thread != tid)
        {
            if (threads_completed == total_threads)
                break;
            pthread_cond_wait(&clock_tick, &pipeline_mutex);
        }

        // break if all threads have completed their work
        if (threads_completed == total_threads)
        {
            pthread_mutex_unlock(&pipeline_mutex);
            break;
        }

        if (!done)
        {
            if (is_stalled)
            {
                // Costly stall has completed, now we can proceed with work
                is_stalled = false;
            }
            else
            {
                // model stalls
                int randValue = intRand(1, 100, data->seed);
                if (randValue < 10) // models costly stall 10% of the time
                {
                    is_stalled = true;
                    total_stalls++;

                    // CGMT: Switch threads ON STALL, incurring a 1-cycle penalty
                    switch_to_next_thread();

                    if (threads_completed < total_threads && current_active_thread != tid)
                    {
                        //printf("[cycle %02d] HW: Context Switch Penalty\n", global_clock);
                        global_clock++;
                    }
                }
                else if (randValue > 80) // models a short stall 20% of the time
                {
                    is_stalled = false;
                    total_stalls++;
                }
                else
                {
                    std::vector<std::string> chunk;
                    if (current_line + 4 > end_line)
                    {
                        chunk.insert(chunk.end(), data->text_chunk.begin() + current_line, data->text_chunk.begin() + end_line);
                        current_line = end_line;
                    }
                    else
                    {
                        chunk.insert(chunk.end(), data->text_chunk.begin() + current_line, data->text_chunk.begin() + current_line + 4);
                        current_line += 4;
                    }
                    processTextChunk(chunk, data->localHashMap);
                    // printf("[cycle %02d] Thread %d: COMPUTE (Line[%d])\n", global_clock, tid, current_line);

                    if (current_line >= end_line)
                    {
                        done = true;
                        threads_completed++;
                        threads_done[tid] = 1; // Mark this thread as done for the scheduler
                        printf("[cycle %02d] Thread %d: FINISHED WORKLOAD\n", global_clock, tid);
                        printf("[cycle %02d] Thread %d: Total stalls = %d\n", global_clock, tid, total_stalls);
                        switch_to_next_thread(); // Move to the next thread immediately after finishing
                    }
                }
            }
        }
        else
        {
            // HW slot is empty, acting as a NOP to keep the pipeline moving for others
            // printf("[cycle %02d] Thread %d: IDLE (Pumping Clock)\n", global_clock, tid);
        }

        global_clock++;
        pthread_cond_broadcast(&clock_tick);
        pthread_mutex_unlock(&pipeline_mutex);
    }
    return NULL;
}

int runMapReduce_coarse(std::string filePath, int numThreads, std::unordered_map<std::string, int> &globalHashMap, int seed)
{
    global_clock = 0;
    threads_done = new int[numThreads](); // Initialize thread done array based on number of threads
    // Read file and get lines
    std::vector<std::string> lines = readFileToLines(filePath);

    if (!lines.empty())
    {
        std::cout << "File read successfully. Number of lines: " << getLineCount(lines) << std::endl;

        size_t totalLines = getLineCount(lines);
        pthread_t threads[numThreads];
        int chunkSize = (getLineCount(lines) + numThreads - 1) / numThreads;
        total_threads = numThreads;
        ThreadData **threadDataArray = new ThreadData *[numThreads]; // Array to hold thread data pointers

        // Map phase: Process the chunk of text and count word occurrences
        for (int i = 0; i < numThreads; ++i)
        {
            size_t start = i * chunkSize;
            size_t end = std::min(start + chunkSize, totalLines);
            std::vector<std::string> textChunk(lines.begin() + start, lines.begin() + end);

            ThreadData *data = new ThreadData;
            data->thread_id = i;
            data->text_chunk = textChunk;
            data->chunk_size = chunkSize;
            data->localHashMap = std::unordered_map<std::string, int>();
            data->seed = seed + i;
            threadDataArray[i] = data; // Store pointer to thread data

            pthread_create(&threads[i], NULL, map, data);
        }

        for (int i = 0; i < numThreads; i++)
        {
            pthread_join(threads[i], NULL);
        }

        // Reduce phase: Add the word counts
        std::cout << "All threads have completed their workloads. Combining results..." << std::endl;
        for (int i = 0; i < numThreads; i++)
        {
            // Do reducer work here
            for (const auto &pair : threadDataArray[i]->localHashMap)
            {
                globalHashMap[pair.first] += pair.second; // Combine counts into the global hash map
            }
        }

        // Clean up thread data
        for (int i = 0; i < numThreads; i++)
        {
            delete threadDataArray[i];
        }
        delete[] threadDataArray;
    }
    else
    {
        std::cout << "Failed to read file." << std::endl;
    }

    total_threads = 0; // Reset total threads for next run
    return 0;
}