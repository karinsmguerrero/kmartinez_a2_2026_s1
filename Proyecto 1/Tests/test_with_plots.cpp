#include <cmath>
#include <matplot/matplot.h>
#include "../FileManagement/FileReader.h"
#include "../Utils/TopWords.h"
#include "../Models/Serial.h"
#include "../Models/FineGrained.h"
#include "../Models/CoarseGrained.h"
#include <string>

void serial_test(std::vector<std::string> lines, int total_runs = 100, int seed = 0)
{
    std::unordered_map<std::string, int> globalHashMap;
    Serial serialModel;

    std::vector<int> run_times(total_runs);

    for (int i = 0; i < total_runs; i++)
    {
        ThreadResults serialResults = serialModel.runMapReduce(lines, globalHashMap, seed);
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

int main(int argc, char *argv[])
{
    std::string files[] = {
        "Assets/conde.txt",
        "Assets/fellowship.txt",
        "Assets/In Search of Lost Time.txt",
        "Assets/merged_files.txt"};

    int fileChoice = 3;

    // printf("------------ Running Serial Model on %s------------ \n", files[fileChoice - 1].c_str());
    std::vector<std::string> lines = readFileToLines(files[fileChoice - 1]);
    if (!lines.empty())
    {
        printf("File read successfully. Number of lines: %d\n", (int)getLineCount(lines));

        int total_runs = argc > 1 ? std::stoi(argv[1]) : 200; // Default to 200 if not specified
        printf("Total runs for histogram: %d\n", total_runs);

        // Models and global hash maps for storing results
        std::unordered_map<std::string, int> globalHashMap_serial;
        Serial serialModel;

        std::unordered_map<std::string, int> globalHashMap_fineGrained;
        FineGrained fineGrainedModel;

        std::unordered_map<std::string, int> globalHashMap_coarseGrained;
        CoarseGrained coarseGrainedModel;

        // std::vector<int> num_threads = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // Different thread counts for scalability testing
        std::vector<int> num_threads = {1, 2, 3}; // Different thread counts for scalability testing

        std::vector<double> avg_speedup_fineGrained(num_threads.size()), avg_speedup_coarseGrained(num_threads.size());
        std::vector<double> avg_cycles_serial(num_threads.size()), avg_cycles_fineGrained(num_threads.size()), avg_cycles_coarseGrained(num_threads.size());
        std::vector<double> efficiency_fineGrained(num_threads.size()), efficiency_coarseGrained(num_threads.size());
        std::vector<std::vector<int>> serial_clock_ticks(num_threads.size()), fineGrained_clock_ticks(num_threads.size()), coarseGrained_clock_ticks(num_threads.size());
        std::vector<std::vector<double>> fineGrained_speedup(num_threads.size()), coarseGrained_speedup(num_threads.size());

        for (auto j = 0; j < num_threads.size(); j++)
        {
            // Array to hold thread result pointers
            ThreadResults *serialResults = new ThreadResults[total_runs];
            ThreadResults *fineGrainedResults = new ThreadResults[total_runs];
            ThreadResults *coarseGrainedResults = new ThreadResults[total_runs];

            std::vector<int> serial_ticks(total_runs), fineGrained_ticks(total_runs), coarseGrained_ticks(total_runs);
            std::vector<double> speedup_fineGrained(total_runs), speedup_coarseGrained(total_runs);

            int numThreads = num_threads[j];
            printf("\n--- Running with %d threads ---\n", numThreads);

            for (int i = 0; i < total_runs; i++)
            {
                printf("Run %d/%d\n", i + 1, total_runs);
                // Seed for random number generator
                int seed = clock() + std::hash<std::thread::id>()(std::this_thread::get_id());
                ThreadResults serial_results = serialModel.runMapReduce(lines, globalHashMap_serial, seed);
                serialResults[i] = serial_results;
                ThreadResults fine_grained_results = fineGrainedModel.runMapReduce(lines, numThreads, globalHashMap_fineGrained, seed);
                fineGrainedResults[i] = fine_grained_results;
                ThreadResults coarse_grained_results = coarseGrainedModel.runMapReduce(lines, numThreads, globalHashMap_coarseGrained, seed);
                coarseGrainedResults[i] = coarse_grained_results;
            }

            // Calculate average clock ticks for each model
            double avg_ticks_serial = 0.0, avg_ticks_fineGrained = 0.0, avg_ticks_coarseGrained = 0.0;
            double avg_sp_fineGrained = 0.0, avg_sp_coarseGrained = 0.0;

            // std::vector<double> speedup_fineGrained(total_runs), speedup_coarseGrained(total_runs);
            std::vector<int> range(total_runs);

            for (int i = 0; i < total_runs; i++)
            {
                avg_ticks_serial += serialResults[i].clock_ticks;
                avg_ticks_fineGrained += fineGrainedResults[i].clock_ticks;
                avg_ticks_coarseGrained += coarseGrainedResults[i].clock_ticks;

                avg_sp_fineGrained += (double)serialResults[i].clock_ticks / fineGrainedResults[i].clock_ticks;
                avg_sp_coarseGrained += (double)serialResults[i].clock_ticks / coarseGrainedResults[i].clock_ticks;

                serial_ticks[i] = serialResults[i].clock_ticks; // Store result for histogram
                fineGrained_ticks[i] = fineGrainedResults[i].clock_ticks;
                coarseGrained_ticks[i] = coarseGrainedResults[i].clock_ticks;

                speedup_fineGrained[i] = (double)serialResults[i].clock_ticks / fineGrainedResults[i].clock_ticks;
                speedup_coarseGrained[i] = (double)serialResults[i].clock_ticks / coarseGrainedResults[i].clock_ticks;
            }

            avg_cycles_serial[j] = avg_ticks_serial / total_runs;
            avg_cycles_fineGrained[j] = avg_ticks_fineGrained / total_runs;
            avg_cycles_coarseGrained[j] = avg_ticks_coarseGrained / total_runs;

            avg_speedup_fineGrained[j] = avg_sp_fineGrained / total_runs;
            avg_speedup_coarseGrained[j] = avg_sp_coarseGrained / total_runs;

            efficiency_fineGrained[j] = avg_speedup_fineGrained[j] / numThreads;
            efficiency_coarseGrained[j] = avg_speedup_coarseGrained[j] / numThreads;

            serial_clock_ticks[j] = serial_ticks;
            fineGrained_clock_ticks[j] = fineGrained_ticks;
            coarseGrained_clock_ticks[j] = coarseGrained_ticks;

            fineGrained_speedup[j] = speedup_fineGrained;
            coarseGrained_speedup[j] = speedup_coarseGrained;

            printf("Average Clock Ticks - Serial: %.2f, Fine-Grained: %.2f, Coarse-Grained: %.2f\n", avg_cycles_serial[j], avg_cycles_fineGrained[j], avg_cycles_coarseGrained[j]);
            printf("Average Speedup - Fine-Grained: %.2f, Coarse-Grained: %.2f\n", avg_speedup_fineGrained[j], avg_speedup_coarseGrained[j]);
            printf("Average Efficiency - Fine-Grained: %.2f, Coarse-Grained: %.2f\n", efficiency_fineGrained[j], efficiency_coarseGrained[j]);

            delete[] serialResults;
            delete[] fineGrainedResults;
            delete[] coarseGrainedResults;
        }

        // Create plots for each model
        auto p = matplot::figure(true);
        p->size(1500, 900);
        matplot::subplot(2, 3, 0);
        matplot::xlabel("Thread Count");
        matplot::ylabel("Average speedup");
        matplot::plot(num_threads, avg_speedup_fineGrained);
        matplot::title("Fine-Grained Model Speedup");

        matplot::subplot(2, 3, 1);
        matplot::xlabel("Thread Count");
        matplot::ylabel("Average cycles");
        matplot::plot(num_threads, avg_cycles_fineGrained);
        matplot::title("Fine-Grained Clock Ticks");

        matplot::subplot(2, 3, 2);
        matplot::xlabel("Thread Count");
        matplot::ylabel("Efficiency");
        matplot::plot(num_threads, efficiency_fineGrained);
        matplot::title("Fine-Grained Efficiency");

        matplot::subplot(2, 3, 3);
        matplot::xlabel("Thread Count");
        matplot::ylabel("Average speedup");
        matplot::plot(num_threads, avg_speedup_coarseGrained);
        matplot::title("Coarse-Grained Model Speedup");

        matplot::subplot(2, 3, 4);
        matplot::xlabel("Thread Count");
        matplot::ylabel("Average cycles");
        matplot::plot(num_threads, avg_cycles_coarseGrained);
        matplot::title("Coarse-Grained Clock Ticks");

        matplot::subplot(2, 3, 5);
        matplot::xlabel("Thread Count");
        matplot::ylabel("Efficiency");
        matplot::plot(num_threads, efficiency_coarseGrained);
        matplot::title("Coarse-Grained Efficiency");

        matplot::save("Performance_analysis/plots/lineplot.jpg");

        // Create histograms for each model
        auto h = matplot::figure(true);
        h->size(600, 600);
        matplot::subplot(3, 1, 0);
        matplot::hist(serial_clock_ticks[0]);
        matplot::title("Serial model Cycles distribution (1 thread)");

        matplot::subplot(3, 1, 1);
        matplot::hist(fineGrained_clock_ticks[0]);
        matplot::title("Fine-Grained model Cycles distribution (1 thread)");

        matplot::subplot(3, 1, 2);
        matplot::hist(coarseGrained_clock_ticks[0]);
        matplot::title("Coarse-Grained model Cycles distribution (1 thread)");
        matplot::save("Performance_analysis/plots/histogram.jpg");

        h->size(600, 600);
        matplot::subplot(3, 1, 0);
        matplot::hist(serial_clock_ticks[0]);
        matplot::title("Serial model Cycles distribution (1 thread)");

        matplot::subplot(3, 1, 1);
        matplot::hist(fineGrained_clock_ticks[0]);
        matplot::title("Fine-Grained model Cycles distribution (1 thread)");

        matplot::subplot(3, 1, 2);
        matplot::hist(coarseGrained_clock_ticks[0]);
        matplot::title("Coarse-Grained model Cycles distribution (1 thread)");
        matplot::save("Performance_analysis/plots/histogram.jpg");

        for (auto i = 0; i < num_threads.size(); i++)
        {
            // Histogram of clock ticks for each model and thread count
            h->size(600, 600);
            matplot::subplot(3, 1, 0);
            matplot::hist(serial_clock_ticks[i]);
            std::string title = "Serial model Cycles distribution (" + std::to_string(num_threads[i]) + " threads)";
            matplot::title(title);

            matplot::subplot(3, 1, 1);
            matplot::hist(fineGrained_clock_ticks[i]);
            title = "Fine-Grained model Cycles distribution (" + std::to_string(num_threads[i]) + " threads)";
            matplot::title(title);

            matplot::subplot(3, 1, 2);
            matplot::hist(coarseGrained_clock_ticks[i]);
            title = "Coarse-Grained model Cycles distribution (" + std::to_string(num_threads[i]) + " threads)";
            matplot::title(title);
            std::string filename = "Performance_analysis/plots/histogram_clock_ticks_" + std::to_string(num_threads[i]) + "_threads.jpg";
            matplot::save(filename);

            // Histogram of speedup for each model and thread count
            h->size(600, 600);
            matplot::subplot(2, 1, 0);
            matplot::hist(fineGrained_speedup[i]);
            title = "Fine-Grained model Speedup distribution (" + std::to_string(num_threads[i]) + " threads)";
            matplot::title(title);

            matplot::subplot(2, 1, 1);
            matplot::hist(coarseGrained_speedup[i]);
            title = "Coarse-Grained model Speedup distribution (" + std::to_string(num_threads[i]) + " threads)";
            matplot::title(title);

            filename = "Performance_analysis/plots/histogram_speedup_" + std::to_string(num_threads[i]) + "_threads.jpg";
            matplot::save(filename);
        }
    }
    else
    {
        printf("Failed to read file.\n");
    }
}

// TODO
/*Para cuantificar la mejora deben proveer al menos las siguientes métricas:
        matplot::title("Serial model clock ticks distribution");
    }
    else
    {
        printf("Failed to read file.\n");
    }
}

// TODO
/*Para cuantificar la mejora deben proveer al menos las siguientes métricas:
a) Tiempo total de ejecución.
b) Speedup respecto a la versión secuencial.
c) Eficiencia paralela.
d ) Escalabilidad en función del número de hilos o procesos.*/