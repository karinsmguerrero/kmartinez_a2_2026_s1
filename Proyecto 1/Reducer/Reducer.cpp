#include "Reducer.h"

void reduce(std::unordered_map<std::string, int> &globalHashMap, ThreadData **threadDataArray, int numThreads)
{
    // printf("All threads have completed their workloads. Combining results...\n");
    for (int i = 0; i < numThreads; i++)
    {
        // Do reducer work here
        for (const auto &pair : threadDataArray[i]->localHashMap)
        {
            globalHashMap[pair.first] += pair.second; // Combine counts into the global hash map
        }
    }
}