# Nombres de los ejecutables
EXEC = mvirtual
EXEC1 = simulapc

# Compilador
CXX = g++

# Directorios
SRC_DIR = parte2
SRC1_DIR = parte1
OBJ_DIR = build

# Archivos fuente y objetos
SRC = $(SRC_DIR)/algoritmos.cpp $(SRC_DIR)/mainParte2.cpp $(SRC_DIR)/hash_table.cpp $(SRC_DIR)/hash_table_pair.cpp
SRC1 = $(SRC1_DIR)/mainParte1.cpp

OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)
OBJ1 = $(SRC1:%.cpp=$(OBJ_DIR)/%.o)

# Regla principal
all: $(EXEC) $(EXEC1)

# Compilar el primer ejecutable
$(EXEC): $(OBJ)
	$(CXX) -o $@ $^

# Compilar el segundo ejecutable
$(EXEC1): $(OBJ1)
	$(CXX) -o $@ $^

# Regla para generar los objetos
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D) # Crea el directorio de los objetos si no existe
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar
clean:
	rm -rf $(OBJ_DIR) $(EXEC) $(EXEC1)
