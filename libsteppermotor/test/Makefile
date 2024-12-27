# Compiler and flags
CXX = g++
CXX_FLAGS = -std=c++17 -Wall -g
# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = ../bin


INCLUDE_DIRS = -Iinclude
INCLUDE_DIRS += -I../../libsteppermotor/include
INCLUDE_DIRS += -I../../libsteppermotor/src

LIB_DIRS =  -L../../libsteppermotor/obj
LIBS = -lsteppermotor #NOTE HOW THIS IS NOT LIBsteppermotor, its Lgain*
LD_FLAGS = $(LIB_DIRS) $(LIBS)

# Source and object files
# Find all source files and corresponding object and binary files
SRC = $(wildcard $(SRC_DIR)/*.cc)
OBJ = $(patsubst $(SRC_DIR)/%.cc,$(OBJ_DIR)/%.o,$(SRC))
BIN = $(patsubst $(SRC_DIR)/%.cc,$(BIN_DIR)/%,$(SRC))

CXX_FLAGS+=$(INCLUDE_DIRS)
# Default rule
all: $(BIN)
# Rule to create each binary
$(BIN_DIR)/%: $(OBJ_DIR)/%.o  | $(BIN_DIR)
	$(CXX) $(CXX_FLAGS) $< -o $@ $(LD_FLAGS)

# Rule to compile source files into object files with dependency generation
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc | $(OBJ_DIR)
	$(CXX) $(CXX_FLAGS) -MMD -c $< -o $@

# Create directories if they don't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean up
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
	rm -rf $(BIN)  

-include $(OBJ:.o=.d)