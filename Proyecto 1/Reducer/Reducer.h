
#ifndef REDUCER_H
#define REDUCER_H   

#include <unordered_map>
#include <string>
#include <vector>
#include "../Models/ThreadData.h"

void reduce(std::unordered_map<std::string, int> &globalHashMap, ThreadData **threadDataArray, int numThreads);
#endif // REDUCER_H