# Nombre del ejecutable
TARGET = main

TARGET_HIST = test_histogram

TARGET_SMT = test_smt

# Archivos fuente
SRC = Tests/test_all_models.cpp FileManagement/FileReader.cpp Mapper/Mapper.cpp Models/Stalls.cpp \
Models/CoarseGrained.cpp Models/Serial.cpp Models/FineGrained.cpp Utils/TopWords.cpp \
Reducer/Reducer.cpp Models/SMT.cpp

SRC_HIST = Tests/test_with_histogram.cpp FileManagement/FileReader.cpp Mapper/Mapper.cpp Models/Stalls.cpp \
Models/Serial.cpp Models/FineGrained.cpp Models/CoarseGrained.cpp Utils/TopWords.cpp \
Reducer/Reducer.cpp Models/SMT.cpp

SRC_SMT = Tests/test_smt.cpp FileManagement/FileReader.cpp Mapper/Mapper.cpp Models/Stalls.cpp \
Utils/TopWords.cpp Reducer/Reducer.cpp Models/SMT.cpp

# Opciones de compilación
CXXFLAGS = -Wall -Wextra -std=c++17
CXXFLAGS_HIST = -Wall -Wextra -std=c++17 -fsanitize=address -g -lmatplot

# Regla por defect
all: $(TARGET)

# Regla para compilar el ejecutable
$(TARGET): $(SRC)
	@echo "Compiling $(TARGET)..."
	@g++ -o build/$(TARGET) $(SRC) $(CXXFLAGS)

# Regla para ejecutar el programa
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
	@./build/$(TARGET_SMT)

# Limpieza de archivos generados
clean:
	@echo "Cleaning up..."
	@rm -f build/$(TARGET) build/$(TARGET_HIST) build/$(TARGET_SMT) *.o