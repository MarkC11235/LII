# CC = g++
CC = clang++-16
CXXFLAGS = -Wall -std=c++17 
LDFLAGS = #-lSDL2
INPUT_FILE = tester.cl
EXE = ./lii

VERSION = BYTECODE

all: build_bytecode runv

build_base:
	@start_time=$$(date +%s); \
	$(CC) $(CXXFLAGS) ./src_base/main.cpp -o $(EXE); \
	end_time=$$(date +%s); \
	elapsed_time=$$((end_time - start_time)); \
	echo "Bytecode version compiled in $$elapsed_time seconds"

build_bytecode:
	@start_time=$$(date +%s); \
	$(CC) $(CXXFLAGS) ./src_bytecode/main.cpp $(LDFLAGS) -o $(EXE) ; \
	end_time=$$(date +%s); \
	elapsed_time=$$((end_time - start_time)); \
	echo "Bytecode version compiled in $$elapsed_time seconds"

run:
	@echo "Running $(INPUT_FILE)\n"
	@$(EXE) $(INPUT_FILE) 

runv:
	@echo "Running $(INPUT_FILE)\n"
	@$(EXE) $(INPUT_FILE) -v

runvT:
	@echo "Running $(INPUT_FILE)\n"
	@$(EXE) $(INPUT_FILE) -vT

runvP:
	@echo "Running $(INPUT_FILE)\n"
	@$(EXE) $(INPUT_FILE) -vP

runvB:
	@echo "Running $(INPUT_FILE)\n"
	@$(EXE) $(INPUT_FILE) -vB

runvV:
	@echo "Running $(INPUT_FILE)\n"
	@$(EXE) $(INPUT_FILE) -vV

time:
	@echo "Running $(INPUT_FILE)\n"
	@$(EXE) $(INPUT_FILE) -t

debug:
	@echo "Running $(INPUT_FILE) in debug mode\n"
	@$(EXE) $(INPUT_FILE) -d -vV

test : build_bytecode
	@for i in $$(find tests_2 -type f -name '*.cl'); do \
		echo "Running test $$i"; \
		$(EXE) $$i > $${i}.temp; \
		diff -b -w $${i}.temp $${i}.out && echo -e "\033[0;32mTest Passed\033[0m" || echo -e "\033[0;31mTest Failed\033[0m"; \
		echo "-----------------------------------"; \
	done

clean:
	@rm -f main.exe
	@for i in tests/*.temp; do \
		rm -f $$i; \
	done