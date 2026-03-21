# Nombre del ejecutable
TARGET = main

# Archivos fuente
SRC = main.cpp FileManagement/FileReader.cpp Mapper/Mapper.cpp Models/Stalls.cpp \
Models/CoarseGrained.cpp Models/Serial.cpp Models/FineGrained.cpp Utils/TopWords.cpp 

SRC_TEST = test.cpp FileManagement/FileReader.cpp Mapper/Mapper.cpp Models/Stalls.cpp \
Models/Serial.cpp

# Opciones de compilación
CXXFLAGS = -Wall -Wextra -std=c++17
CXXFLAGS2 = -Wall -Wextra -std=c++17 -fsanitize=address -g -lmatplot

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

# Compile and run test.cpp
run-test: 
	@echo "Compiling and running test.cpp..."
	@g++ -o test $(SRC_TEST) $(CXXFLAGS2) 
	@./test

# Limpieza de archivos generados
clean:
	@echo "Cleaning up..."
	@rm -f build/$(TARGET) test *.o