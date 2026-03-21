#include <cmath>
#include <matplot/matplot.h>
#include "Models/Serial.h"
#include "FileManagement/FileReader.h"

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
        std::unordered_map<std::string, int> globalHashMap;

        Serial serialModel;
        Serial::ThreadResults serialResults = serialModel.runMapReduce(lines, globalHashMap, seed);
    }
    else
    {
        printf("Failed to read file.\n");
    }
}