

# Executable name 
EXE := Bad_ed_implementation

# Source and build output directories
SRC_DIR := src
INCLUDE_DIR := -I include
OBJ_DIR := obj

# Source files
SRC := $(wildcard $(SRC_DIR)/*.cpp)

# Obj files
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Flags
CPPFLAGS := -Iinclude # Preprocessor flags not C++
CXXFLAGS := -g -O0 -Wall -march=x86-64 -mtune=generic -Wshadow -Wredundant-decls -Wsign-compare $(INCLUDE_DIR)
LDFLAGS := -Llib
LDLIBS := -lm -lstdc++

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir $@

clean:
	$(RM) $(OBJ)