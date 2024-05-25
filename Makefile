CC = g++
CFLAGS = -Wall -std=c++11 
INPUT_FILE = tester.calc

VERSION = BASE

all: build test 

build:
ifeq ($(VERSION), BASE)
	$(CC) $(CFLAGS) ./src_base/main.cpp -o main.exe
	@echo "Base version compiled"
else
	$(error Invalid version: $(VERSION))
endif

run:
	@./main.exe $(INPUT_FILE)

test:
	@for i in tests/*.calc; do \
		echo "Running test $$i"; \
		./main.exe $$i -i './headers/helpers.calc.functions' > $${i}.temp; \
		diff -b -w $${i}.temp $${i}.out && echo -e "\033[0;32mTest Passed\033[0m" || echo -e "\033[0;31mTest Failed\033[0m"; \
		echo "-----------------------------------"; \
	done

test_time:
	@for i in tests/*.calc; do \
		echo "Running test $$i"; \
		./main.exe $$i -v; \
		echo "-----------------------------------"; \
	done

test_time_input:
	echo "Running test $(INPUT_FILE)"
	./main.exe $(INPUT_FILE) -v -i './headers/helpers.calc.functions'

test_speculative_if_time:
	@for i in tests/*.calc; do \
		echo "Running test $$i"; \
		./main.exe $$i -v -sif; \
		diff -b -w $${i}.temp $${i}.out && echo -e "\033[0;32mTest Passed\033[0m" || echo -e "\033[0;31mTest Failed\033[0m"; \
		echo "-----------------------------------"; \
	done

test_speculative_if_time_input:
	echo "Running test $(INPUT_FILE)"
	./main.exe $(INPUT_FILE) -v -sif

clean:
	@rm -f main.exe
	@for i in tests/*.temp; do \
		rm -f $$i; \
	done