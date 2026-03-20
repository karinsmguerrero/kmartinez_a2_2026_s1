# Nombre del ejecutable
TARGET = main

# Archivos fuente
SRC = main.cpp FileManagement/FileReader.cpp Mapper/Mapper.cpp Models/Stalls.cpp Models/Serial.cpp Models/FineGrained.cpp Models/CoarseGrained.cpp

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

# Limpieza de archivos generados
clean:
	@echo "Cleaning up..."
	@rm -f build/$(TARGET) *.o