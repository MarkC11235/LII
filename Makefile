CC = g++
CXXFLAGS = -Wall -std=c++17 
INPUT_FILE = tester.calc
EXE = ./lii

VERSION = BYTECODE

all: build runv

build:
ifeq ($(VERSION), BASE)
	$(CC) $(CXXFLAGS) ./src_base/main.cpp -o $(EXE)
	@echo "Base version compiled"
else ifeq ($(VERSION), BYTECODE)
	$(CC) $(CXXFLAGS) ./src_bytecode/main.cpp -o $(EXE)
	@echo "Bytecode version compiled\n"
else
	$(error Invalid version: $(VERSION))
endif

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

test : build
	@for i in tests_2/*.calc; do \
		echo "Running test $$i"; \
		$(EXE) $$i > $${i}.temp; \
		diff -b -w $${i}.temp $${i}.out && echo -e "\033[0;32mTest Passed\033[0m" || echo -e "\033[0;31mTest Failed\033[0m"; \
		echo "-----------------------------------"; \
	done

test_time:
	@for i in tests/*.calc; do \
		echo "Running test $$i"; \
		$(EXE) $$i -v; \
		echo "-----------------------------------"; \
	done

test_time_input:
	echo "Running test $(INPUT_FILE)"
	$(EXE) $(INPUT_FILE) -v -i './headers/helpers.calc.functions'

test_speculative_if_time:
	@for i in tests/*.calc; do \
		echo "Running test $$i"; \
		$(EXE) $$i -v -sif; \
		diff -b -w $${i}.temp $${i}.out && echo -e "\033[0;32mTest Passed\033[0m" || echo -e "\033[0;31mTest Failed\033[0m"; \
		echo "-----------------------------------"; \
	done

test_speculative_if_time_input:
	echo "Running test $(INPUT_FILE)"
	$(EXE) $(INPUT_FILE) -v -sif

clean:
	@rm -f main.exe
	@for i in tests/*.temp; do \
		rm -f $$i; \
	done