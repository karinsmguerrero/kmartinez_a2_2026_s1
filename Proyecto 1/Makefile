# Nombre del ejecutable
TARGET = main

TARGET_HIST = test_histogram

TARGET_SMT = test_smt

TARGET_CMP = test_cmp

# Archivos fuente
SRC = Tests/test_all_models.cpp FileManagement/FileReader.cpp Mapper/Mapper.cpp Models/Stalls.cpp \
Models/CoarseGrained.cpp Models/Serial.cpp Models/FineGrained.cpp Utils/TopWords.cpp \
Reducer/Reducer.cpp Models/SMT.cpp Models/CMP.cpp

SRC_HIST = Tests/test_with_histogram.cpp FileManagement/FileReader.cpp Mapper/Mapper.cpp Models/Stalls.cpp \
Models/Serial.cpp Models/FineGrained.cpp Models/CoarseGrained.cpp Utils/TopWords.cpp \
Reducer/Reducer.cpp Models/SMT.cpp

SRC_SMT = Tests/test_smt.cpp FileManagement/FileReader.cpp Mapper/Mapper.cpp Models/Stalls.cpp \
Utils/TopWords.cpp Reducer/Reducer.cpp Models/SMT.cpp

SRC_CMP = Tests/test_cmp.cpp FileManagement/FileReader.cpp Mapper/Mapper.cpp Models/Stalls.cpp \
Utils/TopWords.cpp Reducer/Reducer.cpp Models/CMP.cpp

# Opciones de compilación
CXXFLAGS = -Wall -Wextra -std=c++17
CXXFLAGS_HIST = -Wall -Wextra -std=c++17 -fsanitize=address -g -lmatplot

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

# Compile and run test_with_histogram.cpp
run-histogram: $(SRC_HIST)
	@echo "Compiling and running test_with_histogram.cpp..."
	@g++ -o build/$(TARGET_HIST) $(SRC_HIST) $(CXXFLAGS_HIST) 
	@./build/$(TARGET_HIST)

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

# Run perf on test_smt.cpp 
# make perf-smt NUM_THREADS=2
perf-smt:
	@echo "Running perf on test_smt.cpp..."
	@sudo perf stat -r 5 -d build/$(TARGET_SMT) $(NUM_THREADS) 

# Run perf on test_cmp.cpp 
# make perf-cmp NUM_THREADS=2
perf-cmp:
	@echo "Running perf on test_cmp.cpp..."
	@sudo perf stat -r 5 -d build/$(TARGET_CMP) $(NUM_THREADS) 

# Limpieza de archivos generados
clean:
	@echo "Cleaning up..."
	@rm -f build/$(TARGET) build/$(TARGET_HIST) build/$(TARGET_SMT) build/$(TARGET_CMP) *.o