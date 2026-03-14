# Nombre del ejecutable
TARGET = main

# Archivos fuente
SRC = main.cpp FileManagement/FileReader.cpp

# Opciones de compilación
CXXFLAGS = -Wall -Wextra -std=c++11

# Regla por defecto
all: $(TARGET)

# Regla para compilar el ejecutable
$(TARGET): $(SRC)
	@echo "Compiling $(TARGET)..."
	@g++ -o $(TARGET) $(SRC) $(CXXFLAGS)

# Regla para ejecutar el programa
run: $(TARGET)
	@echo "Running $(TARGET)..."
	@./$(TARGET)

# Limpieza de archivos generados
clean:
	@echo "Cleaning up..."
	@rm -f $(TARGET) *.o
