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

TARGET = $(BIN_DIR)/testStepperMotor 

# Source and object files
SRC = $(SRC_DIR)/testStepperMotor.cc
OBJ = $(OBJ_DIR)/testStepperMotor.o
DEPS = $(OBJ:.o=.d)

CXX_FLAGS+=$(INCLUDE_DIRS)
# Default rule
all: $(TARGET)

# Rule to create the executable
$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CXX) $(CXX_FLAGS) $(OBJ) -o $(TARGET) $(LD_FLAGS)

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
	rm -rf $(BIN_DIR)/testStepperMotor
