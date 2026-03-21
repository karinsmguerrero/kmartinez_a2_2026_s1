#include "FineGrained.h"

int FineGrained::global_clock = 0;
int FineGrained::threads_completed = 0; // Tracks when all work is done
int FineGrained::total_threads = 0;     // Total number of threads to be used

pthread_mutex_t FineGrained::pipeline_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t FineGrained::clock_tick = PTHREAD_COND_INITIALIZER;

FineGrained::ThreadResults *FineGrained::map(void *arg)
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

        // FGMT: This thread can only proceed when the global clock modulo NUM_THREADS matches its ID
        while ((global_clock % total_threads) != tid)
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
                int stall_probability = get_random_number(1, 100, data->seed);
                if (stall_probability < 10) // models costly stall 10% of the time
                {
                    is_stalled = true;
                    total_stalls++;
                }
                else if (stall_probability > 80) // models a short stall 20% of the time
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
                        printf("[cycle %02d] Thread %d: FINISHED WORKLOAD\n", global_clock, tid);
                        printf("[cycle %02d] Thread %d: Total stalls = %d\n", global_clock, tid, total_stalls);
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
    return new ThreadResults{global_clock, total_stalls};
}

FineGrained::ThreadResults FineGrained::runMapReduce(std::vector<std::string> lines, int numThreads, std::unordered_map<std::string, int> &globalHashMap, int seed)
{
    global_clock = 0;
    int total_stalls = 0;

    size_t totalLines = lines.size();
    pthread_t threads[numThreads];
    int chunkSize = (totalLines + numThreads - 1) / numThreads;
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

        pthread_create(&threads[i], NULL, (void *(*)(void *))map, data);
    }

    for (int i = 0; i < numThreads; i++)
    {
        ThreadResults *result;
        pthread_join(threads[i], (void **)&result);
        total_stalls += result->total_stalls;
        delete result;
    }

    // Reduce phase: Add the word counts
    printf("All threads have completed their workloads. Combining results...\n");
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

    return ThreadResults{global_clock, total_stalls};
}