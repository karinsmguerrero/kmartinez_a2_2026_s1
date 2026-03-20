#include "Stalls.h"

/*Code taken from: https://stackoverflow.com/questions/21237905/how-do-i-generate-thread-safe-uniform-random-numbers*/
int get_random_number(const int &min, const int &max, int seed)
{
    static thread_local std::mt19937 *generator = nullptr;
    if (!generator)
        generator = new std::mt19937(seed);
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(*generator);
}