#include "SMT.h"

// Code taken from: https://drive.google.com/drive/u/0/folders/1HTw9X3Gxheg50MeacS2Fa3ahSFxTGFie
// Function to bind a thread to a specific CPU core (for SMT simulation)
void bind_thread_to_cpu(int cpu_id)
{
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu_id, &cpuset);
    if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) != 0)
    {
        std::cerr << "Failed to set affinity to CPU " << cpu_id << std::endl;
    }
}

void *SMT::map(void *arg)
{
    ThreadData *data = static_cast<ThreadData *>(arg);
    bind_thread_to_cpu(0); // Bind all threads to the same CPU core
    processTextChunk(data->text_chunk, data->localHashMap);
    return nullptr;  
}

ThreadResults SMT::runMapReduce(std::vector<std::string> lines, int numThreads, std::unordered_map<std::string, int> &globalHashMap)
{
    size_t totalLines = lines.size();
    pthread_t threads[numThreads];
    int chunkSize = (totalLines + numThreads - 1) / numThreads;
    ThreadData **threadDataArray = new ThreadData *[numThreads]; // Array to hold thread data pointers

    // Map phase: Process the chunk of text and count word occurrences
    for (int i = 0; i < numThreads; ++i)
    {
        size_t start = i * chunkSize;
        size_t end = std::min(start + chunkSize, totalLines);
        std::vector<std::string> textChunk(lines.begin() + start, lines.begin() + end);

        ThreadData *data = new ThreadData;
        data->text_chunk = textChunk;
        data->localHashMap = std::unordered_map<std::string, int>();
        threadDataArray[i] = data; // Store pointer to thread data

        if(pthread_create(&threads[i], NULL, (void *(*)(void *))map, data) != 0)
        {
            std::cerr << "Failed to create thread " << i << std::endl;
        }
    }

    for (int i = 0; i < numThreads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Reduce phase: Add the word counts
    reduce(globalHashMap, threadDataArray, numThreads);

    // Clean up thread data
    for (int i = 0; i < numThreads; i++)
    {
        delete threadDataArray[i];
    }
    delete[] threadDataArray;

    return ThreadResults{0, 0};
}
/*void bind_thread_to_cpu(int cpu_id) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(cpu_id, &cpuset);
    if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) != 0) {
        std::cerr << "Failed to set affinity to CPU " << cpu_id << std::endl;
    }
}

void workload(int thread_id, int cpu_id) {
    bind_thread_to_cpu(cpu_id);

    volatile double sum = 0;
    auto start = std::chrono::high_resolution_clock::now();

    // CPU-intensive work
    for (long long i = 0; i < 500000000; ++i) {
        sum += i * 0.0000001;
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(end - start);

    std::cout << "SMT Thread " << thread_id
              << " on CPU " << sched_getcpu()
              << " finished in " << duration.count()
              << " seconds" << std::endl;
}

int main() {
    std::cout << "=== SMT Example: Two threads on same physical core ===" << std::endl;

    // bind both threads to SMT in the same CORE
    std::thread t1(workload, 1, 0);  // Thread 1 on CPU 0
    std::thread t2(workload, 2, 1);  // Thread 2 on CPU 1 (SMT sibling)

    t1.join();
    t2.join();

    return 0;
}*/