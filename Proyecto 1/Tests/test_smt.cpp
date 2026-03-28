#include "../FileManagement/FileReader.h"
#include "../Utils/TopWords.h"
#include "../Models/SMT.h"

int main(int argc, char *argv[])
{
    std::string files[] = {
        "Assets/conde.txt",
        "Assets/fellowship.txt",
        "Assets/In Search of Lost Time.txt",
        "Assets/merged_files.txt"};
    int fileChoice = 3; // Change this to select different files (1, 2, or 3)

    std::vector<std::string> lines = readFileToLines(files[fileChoice - 1]);
    if (!lines.empty())
    {
        printf("File read successfully. Number of lines: %d\n", (int)getLineCount(lines));
        int numThreads = argc > 1 ? std::stoi(argv[1]) : 2; // Default to 2 threads if not specified
        std::unordered_map<std::string, int> globalHashMap;
        SMT smtModel;
        smtModel.runMapReduce(lines, numThreads, globalHashMap);
    }
    else
    {
        printf("Failed to read file.\n");
    }
}