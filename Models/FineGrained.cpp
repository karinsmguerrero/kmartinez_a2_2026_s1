#include <iostream>
#include <string>
#include <algorithm>
#include <pthread.h>
#include "../FileManagement/FileReader.h"
#include "../Mapper/Mapper.h"

int global_clock = 0;
int threads_completed = 0; // Tracks when all work is done
int total_threads = 0; // Total number of threads to be used

pthread_mutex_t pipeline_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t clock_tick = PTHREAD_COND_INITIALIZER;

struct ThreadData {
    int thread_id;
    std::vector<std::string> text_chunk;
    int chunk_size;
    std::unordered_map<std::string, int> localHashMap; // Local hash map for counting words in a chunk
};

void *map(void *arg)
{
    ThreadData *data = static_cast<ThreadData *>(arg);
    int tid = data->thread_id;
    int start_line = tid * data->chunk_size; 
    int end_line = start_line + data->chunk_size  < data->text_chunk.size() ? start_line + data->chunk_size : data->text_chunk.size();
    int i = start_line;
    bool done = false;

    // Loop until ALL threads have finished their workloads
    while (1) {
        pthread_mutex_lock(&pipeline_mutex);

        // Check if everyone is done before waiting
        if (threads_completed == total_threads) {
            pthread_mutex_unlock(&pipeline_mutex);
            break;
        }

        // FGMT: This thread can only proceed when the global clock modulo NUM_THREADS matches its ID
        while ((global_clock % total_threads) != tid) {
            if (threads_completed == total_threads) break; 
            pthread_cond_wait(&clock_tick, &pipeline_mutex);
        }
        
        // break if all threads have completed their work
        if (threads_completed == total_threads) {
            pthread_mutex_unlock(&pipeline_mutex);
            break;
        }

        if (!done) {
            // model stalls with a 20% chance to simulate memory latency
            if (rand() % 100 < 20) {
                printf("[cycle %02d] Thread %d: STALL (Memory Wait/NOP)\n", global_clock, tid);
            } 
            else {
                processTextChunk(std::vector<std::string>(data->text_chunk.begin() + start_line, data->text_chunk.begin() + end_line), data->localHashMap);
                printf("[cycle %02d] Thread %d: COMPUTE (A[%d]*B[%d])\n", global_clock, tid, i, i);
                i++;
                
                if (i == end_line) {
                    done = true;
                    threads_completed++;
                    printf("[cycle %02d] Thread %d: FINISHED WORKLOAD\n", global_clock, tid);
                }
            }
        } else {
            // HW slot is empty, acting as a NOP to keep the pipeline moving for others
            printf("[cycle %02d] Thread %d: IDLE (Pumping Clock)\n", global_clock, tid);
        }

        global_clock++;
        pthread_cond_broadcast(&clock_tick);
        pthread_mutex_unlock(&pipeline_mutex);
    }
    delete data;
    return NULL;
}

int runMapReduce_fine(std::string filePath, int numThreads, std::unordered_map<std::string, int> &globalHashMap)
{
    // Read file and get lines
    std::vector<std::string> lines = readFileToLines(filePath);

    if (!lines.empty())
    {
        std::cout << "File read successfully. Number of lines: " << getLineCount(lines) << std::endl;

        size_t totalLines = getLineCount(lines);
        pthread_t threads[numThreads];
        int chunkSize = (getLineCount(lines) + numThreads - 1) / numThreads; // Calculate chunk size for each thread
        int tids[numThreads];
        total_threads = numThreads;

        // Map phase: Process the chunk of text and count word occurrences
        for (size_t i = 0; i < numThreads; ++i)
        {
            size_t start = i * chunkSize;
            size_t end = std::min(start + chunkSize, totalLines);
            std::vector<std::string> textChunk(lines.begin() + start, lines.begin() + end);

            ThreadData *data = new ThreadData;
            data->thread_id = i;
            data->text_chunk = textChunk;
            data->chunk_size = chunkSize;
            data->localHashMap = std::unordered_map<std::string, int>();

            tids[i] = i;
            pthread_create(&threads[i], NULL, map, data);
        }

        for (int i = 0; i < numThreads; i++)
        {
            pthread_join(threads[i], NULL);
        }

        // Reduce phase: Add the word counts
        int final_res = 0;
        for (int i = 0; i < numThreads; i++)
        {
            // Do reducer work here
        }
    }
    else
    {
        std::cout << "Failed to read file." << std::endl;
    }

    total_threads = 0; // Reset total threads for next run
    return 0;
}

// TO DO
/*
1. Take text vector and divide it into smaller vectors of a specified size (e.g., 100 lines each).
2. Create a thread for each smaller vector to process it concurrently.
3. Each thread will perform the following tasks:
    a. Count the occurrences of each word in its assigned vector.
    b. Store the word counts in a local data structure (e.g., a map or dictionary).
4. After all threads have completed their processing, the main thread will:
    a. Collect the word counts from each thread.
    b. Combine the counts to get the total occurrences of each word across all vectors.
5. Finally, the program will output the total word counts.
*/

/*

        size_t chunkSize = 100; // Number of lines per chunk
        size_t totalLines = getLineCount(lines);
        size_t numChunks = (totalLines + chunkSize - 1) / chunkSize; // Calculate number of chunks needed

        for (size_t i = 0; i < numChunks; ++i)
        {
            size_t start = i * chunkSize;
            size_t end = std::min(start + chunkSize, totalLines);
            std::vector<std::string> textChunk(lines.begin() + start, lines.begin() + end);
            processTextChunk(textChunk); // Process the chunk of text
        }
*/
