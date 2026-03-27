# Nombre del ejecutable
TARGET = main

TARGET_PLOT = test_plot

TARGET_SMT = test_smt

TARGET_CMP = test_cmp

TARGET_FLUSH = flush_cache

# Archivos fuente
SRC = Tests/test_all_models.cpp FileManagement/FileReader.cpp Mapper/Mapper.cpp Models/Stalls.cpp \
Models/CoarseGrained.cpp Models/Serial.cpp Models/FineGrained.cpp Utils/TopWords.cpp \
Reducer/Reducer.cpp Models/SMT.cpp Models/CMP.cpp

SRC_PLOT = Tests/test_with_plots.cpp FileManagement/FileReader.cpp Mapper/Mapper.cpp Models/Stalls.cpp \
Models/Serial.cpp Models/FineGrained.cpp Models/CoarseGrained.cpp Utils/TopWords.cpp \
Reducer/Reducer.cpp Models/SMT.cpp

SRC_SMT = Tests/test_smt.cpp FileManagement/FileReader.cpp Mapper/Mapper.cpp Models/Stalls.cpp \
Utils/TopWords.cpp Reducer/Reducer.cpp Models/SMT.cpp

SRC_CMP = Tests/test_cmp.cpp FileManagement/FileReader.cpp Mapper/Mapper.cpp Models/Stalls.cpp \
Utils/TopWords.cpp Reducer/Reducer.cpp Models/CMP.cpp

SRC_FLUSH = Utils/FlushCache.cpp

# Opciones de compilación
CXXFLAGS = -Wall -Wextra -std=c++17
CXXFLAGS_PLOT = -Wall -Wextra -std=c++17 -fsanitize=address -g -lmatplot

# Regla por defect
all: $(TARGET)

# Regla para compilar el ejecutable
$(TARGET): $(SRC)
	@echo "Compiling $(TARGET)..."
	@g++ -o build/$(TARGET) $(SRC) $(CXXFLAGS)

# Compile and run the main executable with all models
run: $(TARGET)
	@echo "Running $(TARGET)..." 
	@./build/$(TARGET)

# Compile and run test_with_histogram.cpp / make run-histogram NUM_RUNS=20
run-plots: $(SRC_PLOT)
	@echo "Compiling and running test_with_plots.cpp..."
	@g++ -o build/$(TARGET_PLOT) $(SRC_PLOT) $(CXXFLAGS_PLOT) 
	@./build/$(TARGET_PLOT) $(NUM_RUNS)

# Compile and run test_smt.cpp
run-smt: $(SRC_SMT)
	@echo "Compiling and running test_smt.cpp..."
	@g++ -o build/$(TARGET_SMT) $(SRC_SMT) $(CXXFLAGS) 
	@./build/$(TARGET_SMT) $(NUM_THREADS)

# Compile and run test_cmp.cpp
run-cmp: $(SRC_CMP)
	@echo "Compiling and running test_cmp.cpp..."
	@g++ -o build/$(TARGET_CMP) $(SRC_CMP) $(CXXFLAGS) 
	@./build/$(TARGET_CMP) $(NUM_THREADS)

# Clean cache after execution to prevent benchmarking fluctuations
run-flush: $(SRC_FLUSH)
	@echo "Compiling and running flush_cache.cpp..."
	@g++ -o build/$(TARGET_FLUSH) $(SRC_FLUSH) $(CXXFLAGS) 
	@./build/$(TARGET_FLUSH)

# Run perf on test_smt.cpp 
# make perf-smt NUM_THREADS=2
perf-smt:
	@echo "Running perf on test_smt.cpp..."
	@sudo perf stat -e cpu-cycles,context-switches,cache-misses,stalled-cycles-frontend -r 5 -o Performance_analysis/output_smt_$(NUM_THREADS).txt -d build/$(TARGET_SMT) $(NUM_THREADS) 
	@echo 3 | sudo tee /proc/sys/vm/drop_caches
	@./build/$(TARGET_FLUSH)

# Run perf on test_cmp.cpp 
# make perf-cmp NUM_THREADS=2
perf-cmp:
	@echo "Running perf on test_cmp.cpp..."
	@sudo perf stat -r 200 -o Performance_analysis/output_cmp_$(NUM_THREADS).txt -d build/$(TARGET_CMP) $(NUM_THREADS) 
	@echo 3 | sudo tee /proc/sys/vm/drop_caches
	@./build/$(TARGET_FLUSH)

# Limpieza de archivos generados
clean:
	@echo "Cleaning up..."
	@rm -f build/$(TARGET) build/$(TARGET_HIST) build/$(TARGET_SMT) build/$(TARGET_CMP) *.o

#cache-misses cpu-cycles context-switches 
# sudo perf stat -r 10 -e cache-misses,cpu-cycles,context-switches -o Performance_analysis/output_smt_3.txt -d build/test_smt 3