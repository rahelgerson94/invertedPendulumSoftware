# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17 -Wall -Wextra  -g -Wc++11-extensions

# Directories
SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = ../lib
LIBRARY = $(LIB_DIR)/libsignalgenerator.a

INCLUDE_DIRS =  -Iinclude/
INCLUDE_DIRS += -I../libutil/include

# Source files
SRC = $(SRC_DIR)/SignalGenerator.cc
OBJ = $(OBJ_DIR)/SignalGenerator.o

CXXFLAGS+= $(INCLUDE_DIRS)
# Default LIBRARY
all: $(LIBRARY)

# Rule to create the static library
$(LIBRARY): $(OBJ) | $(OBJ_DIR)
	ar rcs $(LIBRARY) $(OBJ)

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

# Create directories if they don't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean up
clean:
	rm -rf $(OBJ_DIR)
	rm $(LIBRARY)

-include $(OBJ:.o=.d)
