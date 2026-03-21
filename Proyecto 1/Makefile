# Nombre del ejecutable
TARGET = main

# Archivos fuente
SRC = main.cpp FileManagement/FileReader.cpp Mapper/Mapper.cpp Models/Stalls.cpp \
Models/CoarseGrained.cpp Models/Serial.cpp Models/FineGrained.cpp Utils/TopWords.cpp

# Opciones de compilación
CXXFLAGS = -Wall -Wextra -std=c++11

# Regla por defecto
all: $(TARGET)

# Regla para compilar el ejecutable
$(TARGET): $(SRC)
	@echo "Compiling $(TARGET)..."
	@g++ -o build/$(TARGET) $(SRC) $(CXXFLAGS)

# Regla para ejecutar el programa
run: $(TARGET)
	@echo "Running $(TARGET)..." 
	@./build/$(TARGET)

# Test target with automatic inputs (model 3: Coarse Grained, text 1: El Conde)
test: $(TARGET)
	@echo "Running automated test (Coarse Grained Model with El Conde)..."
	@echo -e "3\n1" | ./build/$(TARGET)

# Test target with Serial Model and Fellowship
test-serial: $(TARGET)
	@echo "Running automated test (Serial Model with Fellowship)..."
	@echo -e "1\n2" | ./build/$(TARGET)

# Test target with Fine Grained Model and In Search of Lost Time
test-fine: $(TARGET)
	@echo "Running automated test (Fine Grained Model with In Search of Lost Time)..."
	@echo -e "2\n3" | ./build/$(TARGET)

# Limpieza de archivos generados
clean:
	@echo "Cleaning up..."
	@rm -f build/$(TARGET) *.o