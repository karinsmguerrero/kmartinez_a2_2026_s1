#include "CMP.h"

void *CMP::map(void *arg)
{
    ThreadData *data = static_cast<ThreadData *>(arg);
    processTextChunk(data->text_chunk, data->localHashMap);
    return nullptr;  
}

ThreadResults CMP::runMapReduce(std::vector<std::string> lines, int numThreads, std::unordered_map<std::string, int> &globalHashMap)
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
