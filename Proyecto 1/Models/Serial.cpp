#include "Serial.h"


int Serial::runMapReduce(std::string filePath, std::unordered_map<std::string, int> &localHashMap, int seed)
{
    int clock_ticks = 0;
    // Read file and get lines
    std::vector<std::string> lines = readFileToLines(filePath);

    if (!lines.empty())
    {
        std::cout << "File read successfully. Number of lines: " << getLineCount(lines) << std::endl;

        int start_line = 0;
        int end_line = lines.size();
        int current_line = start_line;
        bool done = false;
        bool is_stalled = false;
        int total_stalls = 0;

        // Map phase: Process the chunk of text and count word occurrences
        while (!done)
        {
            if (is_stalled)
            {
                // Costly stall has completed, now we can proceed with work
                is_stalled = false;
            }
            else
            {
                // model stalls
                int randValue = get_random_number(1, 100, seed);
                if (randValue < 10) // models costly stall 10% of the time
                {
                    is_stalled = true;
                    total_stalls++;
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
                        chunk.insert(chunk.end(), lines.begin() + current_line, lines.begin() + end_line);
                        current_line = end_line;
                    }
                    else
                    {
                        chunk.insert(chunk.end(), lines.begin() + current_line, lines.begin() + current_line + 4);
                        current_line += 4;
                    }

                    processTextChunk(chunk, localHashMap);

                    if (current_line >= end_line)
                    {
                        done = true;
                        printf("[cycle %02d] Single Thread - FINISHED WORKLOAD\n", clock_ticks);
                        printf("[cycle %02d] Single Thread - Total stalls = %d\n", clock_ticks, total_stalls);
                    }
                }
            }

            clock_ticks++;
        }

        // Reduce phase: Add the word counts
        // No Reduce phase needed for the serial model since we are directly updating the localHashMap in the Mapper
        // and there are no other threads to combine results with.
    }
    else
    {
        std::cout << "Failed to read file." << std::endl;
    }

    return 0;
}