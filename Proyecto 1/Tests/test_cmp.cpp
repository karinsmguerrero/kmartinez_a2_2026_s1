#include "../FileManagement/FileReader.h"
#include "../Utils/TopWords.h"
#include "../Models/CMP.h"

int main(int argc, char *argv[])
{
    std::string files[] = {
        "Assets/conde.txt",
        "Assets/fellowship.txt",
        "Assets/In Search of Lost Time.txt"};
    int fileChoice = 1; // Change this to select different files (1, 2, or 3)

    // printf("------------ Running Serial Model on %s------------ \n", files[fileChoice - 1].c_str());
    std::vector<std::string> lines = readFileToLines(files[fileChoice - 1]);
    if (!lines.empty())
    {
        printf("File read successfully. Number of lines: %d\n", (int)getLineCount(lines));
        int numThreads = argv[1] ? argc > 0 : 4; // Default to 4 threads if not specified
        std::unordered_map<std::string, int> globalHashMap;
        CMP cmpModel;
        cmpModel.runMapReduce(lines, numThreads, globalHashMap);
        printf("Top 10 most common words in %s:\n", files[fileChoice - 1].c_str());
        printTopKWordCounts(getTopKWords(globalHashMap, 10), globalHashMap);
    }
    else
    {
        printf("Failed to read file.\n");
    }
}