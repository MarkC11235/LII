CC = clang++-16
CXXFLAGS = -Wall -std=c++17 -I$(SRC_DIR)
LDFLAGS = -lSDL2
INPUT_FILE = tester.cl
CL_EXE_FILE = $(INPUT_FILE:.cl=.cl_exe)

# Define both executables
COMPILER_EXE = ./liic
VM_EXE = ./liivm

SRC_DIR = ./src_bytecode

# Common source files
COMMON_SRCS = $(wildcard $(SRC_DIR)/helpers/*.cpp) \
              $(wildcard $(SRC_DIR)/Tokenizer/*.cpp) \
              $(wildcard $(SRC_DIR)/Parser/*.cpp) \
              $(wildcard $(SRC_DIR)/BytecodeGenerator/*.cpp) \
              $(wildcard $(SRC_DIR)/FlagParser/*.cpp) \
              $(wildcard $(SRC_DIR)/std_lib/*.cpp) \
              $(wildcard $(SRC_DIR)/std_lib/*/*.cpp) \
			  $(wildcard $(SRC_DIR)/CLEXEGenerator/*.cpp) \

# Specific source files for each executable
COMPILER_SRCS = $(SRC_DIR)/compiler.cpp $(COMMON_SRCS)
VM_SRCS = $(SRC_DIR)/vm.cpp $(COMMON_SRCS)

# Object files
COMPILER_OBJS = $(COMPILER_SRCS:.cpp=.o)
VM_OBJS = $(VM_SRCS:.cpp=.o)

# Default target builds both executables
all: build run

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

build: clean $(COMPILER_EXE) $(VM_EXE)

clean:
	@rm -f $(COMPILER_OBJS) $(VM_OBJS) $(COMPILER_EXE) $(VM_EXE)
	@rm -f tests/*.temp
	@rm -f tests_2/*.temp
	@rm -f jit_functions/*

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