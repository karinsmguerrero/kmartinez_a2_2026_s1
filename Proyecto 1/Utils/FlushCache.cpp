#include "FlushCache.h"

void flush_cache() {
    // 2x L3 cache (16 MiB → 32 MiB)
    const size_t size = 32 * 1024 * 1024; // bytes
    const size_t elements = size / sizeof(uint64_t);

    static std::vector<uint64_t> buffer(elements);

    // Paso 1: escribir (evita páginas "lazy zero")
    for (size_t i = 0; i < elements; i++) {
        buffer[i] = i;
    }

    // Paso 2: acceso aleatorio para evitar optimizaciones del hardware
    volatile uint64_t sink = 0;

    std::mt19937_64 rng(12345);
    std::uniform_int_distribution<size_t> dist(0, elements - 1);

    // Tocar muchas veces para asegurar eviction
    for (size_t i = 0; i < elements * 10; i++) {
        sink += buffer[dist(rng)];
    }

    // Evitar que el compilador elimine el código
    (void)sink;
}

int main() {
    flush_cache();
    return 0;
}