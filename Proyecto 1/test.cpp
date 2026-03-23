#include <cmath>
#include <matplot/matplot.h>
#include "FileManagement/FileReader.h"
#include "Utils/TopWords.h"
#include "Models/Serial.h"
#include "Models/FineGrained.h"
#include "Models/CoarseGrained.h"
#include "Models/SMT.h"

void serial_test(std::vector<std::string> lines, int total_runs = 100, int seed = 0)
{
    std::unordered_map<std::string, int> globalHashMap;
    Serial serialModel;

    std::vector<int> run_times(total_runs);

    for (int i = 0; i < total_runs; i++)
    {
        Serial::ThreadResults serialResults = serialModel.runMapReduce(lines, globalHashMap, seed);
        run_times[i] = serialResults.clock_ticks;
        printf("Run %d: Clock Ticks = %d, Total Stalls = %d\n", i + 1, serialResults.clock_ticks, serialResults.total_stalls);
        globalHashMap.clear();
    }

    // Create a histogram of run times
    auto h = matplot::hist(run_times);
    std::cout << "Histogram with " << h->num_bins() << " bins" << std::endl;
    matplot::show();
}

void fine_grained_test(std::vector<std::string> lines, int numThreads, int total_runs = 100, int seed = 0)
{
    std::unordered_map<std::string, int> globalHashMap;
    FineGrained fineGrainedModel;

    std::vector<int> run_times(total_runs);

    for (int i = 0; i < total_runs; i++)
    {
        ThreadResults fineGrainedResults = fineGrainedModel.runMapReduce(lines, numThreads, globalHashMap, seed);
        run_times[i] = fineGrainedResults.clock_ticks;
        printf("Run %d: Clock Ticks = %d, Total Stalls = %d\n", i + 1, fineGrainedResults.clock_ticks, fineGrainedResults.total_stalls);
        /*if (i == 0 || i == total_runs - 1) // Print top words for the first and last run to verify results
        {
            printTopKWordCounts(getTopKWords(globalHashMap, 10), globalHashMap);
        }*/
        globalHashMap.clear();
    }

    // Create a histogram of run times
    auto h = matplot::hist(run_times);
    std::cout << "Histogram with " << h->num_bins() << " bins" << std::endl;
    matplot::show();
}

void coarse_grained_test(std::vector<std::string> lines, int numThreads, int total_runs = 100, int seed = 0)
{
    std::unordered_map<std::string, int> globalHashMap;
    CoarseGrained coarseGrainedModel;

    std::vector<int> run_times(total_runs);

    for (int i = 0; i < total_runs; i++)
    {
        ThreadResults coarseGrainedResults = coarseGrainedModel.runMapReduce(lines, numThreads, globalHashMap, seed);
        run_times[i] = coarseGrainedResults.clock_ticks;
        printf("Run %d: Clock Ticks = %d, Total Stalls = %d\n", i + 1, coarseGrainedResults.clock_ticks, coarseGrainedResults.total_stalls);
        globalHashMap.clear();
    }

    // Create a histogram of run times
    auto h = matplot::hist(run_times);
    std::cout << "Histogram with " << h->num_bins() << " bins" << std::endl;
    matplot::show();
}

void SMT_test(std::vector<std::string> lines, int numThreads, int total_runs = 100)
{
    std::unordered_map<std::string, int> globalHashMap;
    SMT smtModel;
    smtModel.runMapReduce(lines, numThreads, globalHashMap);

    /*std::vector<int> run_times(total_runs);

    for (int i = 0; i < total_runs; i++)
    {
        ThreadResults smtResults = smtModel.runMapReduce(lines, numThreads, globalHashMap);
        run_times[i] = smtResults.clock_ticks;
        printf("Run %d: Clock Ticks = %d, Total Stalls = %d\n", i + 1, smtResults.clock_ticks, smtResults.total_stalls);
        globalHashMap.clear();
    }

    // Create a histogram of run times
    auto h = matplot::hist(run_times);
    std::cout << "Histogram with " << h->num_bins() << " bins" << std::endl;
    matplot::show();*/
}

int main()
{
    std::string files[] = {
        "Assets/conde.txt",
        "Assets/fellowship.txt",
        "Assets/In Search of Lost Time.txt"};

    int fileChoice = 1;

    // Seed for random number generator
    int seed = clock() + std::hash<std::thread::id>()(std::this_thread::get_id());

    // printf("------------ Running Serial Model on %s------------ \n", files[fileChoice - 1].c_str());
    std::vector<std::string> lines = readFileToLines(files[fileChoice - 1]);
    if (!lines.empty())
    {
        printf("File read successfully. Number of lines: %d\n", (int)getLineCount(lines));
        int numThreads = 10;
        int total_runs = 100;
        //serial_test(lines, total_runs, seed);
        //fine_grained_test(lines, numThreads, total_runs, seed);
        //coarse_grained_test(lines, numThreads, total_runs, seed);
        SMT_test(lines, numThreads, total_runs);
    }
    else
    {
        printf("Failed to read file.\n");
    }
}