CC = clang++
CXXFLAGS = -Wall -std=c++17 -I$(SRC_DIR)
LDFLAGS = -lSDL2 -lSDL2_ttf
INPUT_FILE = tester.cl
CL_EXE_FILE = $(INPUT_FILE:.cl=.cl_exe)

# Define both executables
COMPILER_EXE = ./liic
VM_EXE = ./liivm

SRC_DIR = ./src_bytecode

# Remove embedded font from COMMON_SRCS by filtering it out
COMMON_SRCS = $(filter-out $(EMBEDDED_FONT_CPP), \
			  $(wildcard $(SRC_DIR)/helpers/*.cpp) \
              $(wildcard $(SRC_DIR)/Tokenizer/*.cpp) \
              $(wildcard $(SRC_DIR)/Parser/*.cpp) \
              $(wildcard $(SRC_DIR)/BytecodeGenerator/*.cpp) \
              $(wildcard $(SRC_DIR)/FlagParser/*.cpp) \
              $(wildcard $(SRC_DIR)/std_lib/*.cpp) \
              $(wildcard $(SRC_DIR)/std_lib/*/*.cpp) \
              $(wildcard $(SRC_DIR)/CLEXEGenerator/*.cpp) \
              $(wildcard $(SRC_DIR)/DummyOptimizer/*.cpp))

# Specific source files for each executable
COMPILER_SRCS = $(SRC_DIR)/compiler.cpp $(COMMON_SRCS)
VM_SRCS = $(SRC_DIR)/vm.cpp $(COMMON_SRCS)

# Object files
EMBEDDED_FONT_OBJ = $(EMBEDDED_FONT_CPP:.cpp=.o)
COMPILER_OBJS = $(COMPILER_SRCS:.cpp=.o) $(EMBEDDED_FONT_OBJ)
VM_OBJS = $(VM_SRCS:.cpp=.o) $(EMBEDDED_FONT_OBJ)

# Default target builds both executables
all: build run

# Font setup
FONT_FILE = ./resources/fonts/SwanseaItalic-AwqD.ttf
EMBEDDED_FONT_HPP = ./src_bytecode/std_lib/graphics/embedded_font.hpp
EMBEDDED_FONT_CPP = ./src_bytecode/std_lib/graphics/embedded_font.cpp

# Generate both header and source files for embedded font
$(EMBEDDED_FONT_CPP) $(EMBEDDED_FONT_HPP): 
	@echo "Generating embedded font files..."
	@mkdir -p $(dir $@)
	python3 tools/ttf_to_cpp.py $(FONT_FILE) $(EMBEDDED_FONT_CPP) $(EMBEDDED_FONT_HPP)

# Add specific rule for embedded font object
$(EMBEDDED_FONT_OBJ): $(EMBEDDED_FONT_CPP) $(EMBEDDED_FONT_HPP)
	@echo "Compiling embedded font..."
	$(CC) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp # compiles all .cpp files into .o files for each file in SRCS, then they are linked together to form the executable
	$(CC) $(CXXFLAGS) -c $< -o $@

$(COMPILER_EXE): $(COMPILER_OBJS)
	@echo "Linking compiler..."
	@$(CC) -o $(COMPILER_EXE) $(COMPILER_OBJS) $(LDFLAGS)
	@echo "Compiler executable created: $(COMPILER_EXE)"

$(VM_EXE): $(VM_OBJS)
	@echo "Linking VM..."
	@$(CC) -o $(VM_EXE) $(VM_OBJS) $(LDFLAGS)
	@echo "VM executable created: $(VM_EXE)"

build: $(EMBEDDED_FONT_OBJ) $(COMPILER_EXE) $(VM_EXE) 
	@echo "Build completed."

clean:
	@rm -f $(COMPILER_OBJS) $(VM_OBJS) $(COMPILER_EXE) $(VM_EXE) 
	@rm -f $(EMBEDDED_FONT_CPP) $(EMBEDDED_FONT_HPP) $(EMBEDDED_FONT_OBJ)
	@rm -f tests/*.temp
	@rm -f tests_2/*.temp
	@rm -f jit_functions/*
	@rm -f *.cl_exe
	@echo "Cleaned up object files and executables"

# Updated run commands for separate executables
run_compiler:
	@echo "Running compiler on $(INPUT_FILE)"
	@$(COMPILER_EXE) $(INPUT_FILE)

run_vm:
	@echo "Running VM on $(CL_EXE_FILE)"
	@$(VM_EXE) $(CL_EXE_FILE)

run: run_compiler run_vm

test : build
	@echo "Running tests"
	./Makefile_helpers/test.sh $(VM_EXE) tests_2

count:
	@echo "Counting lines of cpp and hpp code"
	@find ./src_bytecode/ -name '*.cpp' -o -name '*.hpp' | xargs wc -l
	@echo "Counting lines of cl and clh code"
	@find ./ -name '*.cl' -o -name '*.clh' | xargs wc -l
