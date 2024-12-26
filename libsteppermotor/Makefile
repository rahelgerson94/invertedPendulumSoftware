# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra  -g -Wc++11-extensions

# Directories
SRC_DIR = src
OBJ_DIR = obj
LIBRARY = $(OBJ_DIR)/libsteppermotor.a

INCLUDE_DIRS =  -Iinclude/

# Source files
SRC = $(SRC_DIR)/StepperMotor.cc
OBJ = $(OBJ_DIR)/StepperMotor.o

CXXFLAGS+= $(INCLUDE_DIRS)
# Default LIBRARY
all: $(LIBRARY)

# Rule to create the static library
$(LIBRARY): $(OBJ) | $(OBJ_DIR)
	ar rcs $(LIBRARY) $(OBJ)

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directories if they don't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean up
clean:
	rm -rf $(OBJ_DIR)
