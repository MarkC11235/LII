CC = clang++-16
CXXFLAGS = -Wall -std=c++17 -I$(SRC_DIR)
LDFLAGS = -lSDL2
INPUT_FILE = tester.cl
EXE = ./lii

SRC_DIR = ./src_bytecode
SRCS = $(wildcard $(SRC_DIR)/*.cpp) \
       $(wildcard $(SRC_DIR)/Tokenizer/*.cpp) \
       $(wildcard $(SRC_DIR)/Parser/*.cpp) \
       $(wildcard $(SRC_DIR)/BytecodeGenerator/*.cpp) \
       $(wildcard $(SRC_DIR)/VM/*.cpp) \
       $(wildcard $(SRC_DIR)/FlagParser/*.cpp) \
	   $(wildcard $(SRC_DIR)/helpers/*.cpp) \
       $(wildcard $(SRC_DIR)/std_lib/*.cpp) \
       $(wildcard $(SRC_DIR)/std_lib/*/*.cpp)
OBJS = $(SRCS:.cpp=.o)

%.o: %.cpp # compiles all .cpp files into .o files for each file in SRCS, then they are linked together to form the executable
	$(CC) $(CXXFLAGS) -c $< -o $@

all: build_bytecode runv

build_bytecode:
	@echo "Starting compilation..."
	@total_start=`date +%s%N`; \
    for src in $(SRCS); do \
        obj=`echo $$src | sed 's/\.cpp/\.o/'`; \
        start=`date +%s%N`; \
        $(CC) $(CXXFLAGS) -c $$src -o $$obj; \
        end=`date +%s%N`; \
        elapsed=`expr \( $$end - $$start \) / 1000000`; \
        sec=`expr $$elapsed / 1000`; \
        msec=`expr $$elapsed % 1000`; \
        printf "Compiled %-70s in %d.%03d seconds\n" $$src $$sec $$msec; \
    done; \
    echo "Linking..."; \
    link_start=`date +%s%N`; \
    $(CC) -o $(EXE) $(OBJS) $(LDFLAGS); \
    link_end=`date +%s%N`; \
    link_time=`expr \( $$link_end - $$link_start \) / 1000000`; \
    total_time=`expr \( $$link_end - $$total_start \) / 1000000`; \
    link_sec=`expr $$link_time / 1000`; \
    link_msec=`expr $$link_time % 1000`; \
    total_sec=`expr $$total_time / 1000`; \
    total_msec=`expr $$total_time % 1000`; \
    printf "Linking completed in %d.%03d seconds\n" $$link_sec $$link_msec; \
    printf "Total compilation time: %d.%03d seconds\n" $$total_sec $$total_msec


run_jit: build_bytecode
	@echo "Running $(INPUT_FILE)\n"
	@$(EXE) $(INPUT_FILE) -jit -v

compare_normal_jit_times: build_bytecode
	@echo "Running $(INPUT_FILE) in normal mode\n"
	@start_time=$$(date +%s); \
	$(EXE) $(INPUT_FILE); \
	end_time=$$(date +%s); \
	normal_elapsed_time=$$((end_time - start_time)); \
	echo "Running $(INPUT_FILE) in JIT mode\n"; \
	start_time=$$(date +%s); \
	$(EXE) $(INPUT_FILE) -jit; \
	end_time=$$(date +%s); \
	jit_elapsed_time=$$((end_time - start_time)); \
	echo "Normal mode took $$normal_elapsed_time seconds"; \
	echo "JIT mode took $$jit_elapsed_time seconds"

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
	@echo "Timing $(INPUT_FILE)\n"
	@$(EXE) $(INPUT_FILE) -t

time_jit:
	@echo "Timing $(INPUT_FILE) with jit enabled\n"
	@$(EXE) $(INPUT_FILE) -jit 1 -t

debug:
	@echo "Running $(INPUT_FILE) in debug mode\n"
	@$(EXE) $(INPUT_FILE) -d -vV

test : build_bytecode
	@echo "Running tests"
	./Makefile_helpers/test.sh $(EXE) tests_2

test_jit : build_bytecode
	@echo "Running tests with JIT enabled"
	./Makefile_helpers/test.sh $(EXE) tests_2 -jit 1

test_all : build_bytecode test test_jit

leak_test : build_bytecode
	@for i in $$(find tests_2 -type f -name '*.cl'); do \
		echo "Running test $$i"; \
		valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes $(EXE) $$i > $${i}.temp; \
		diff -b -w $${i}.temp $${i}.out && echo -e "\033[0;32mTest Passed\033[0m" || echo -e "\033[0;31mTest Failed\033[0m"; \
		echo "-----------------------------------"; \
	done

count:
	@echo "Counting lines of cpp and hpp code"
	@find ./src_bytecode/ -name '*.cpp' -o -name '*.hpp' | xargs wc -l
	@echo "Counting lines of cl and clh code"
	@find ./ -name '*.cl' -o -name '*.clh' | xargs wc -l

clean:
	@rm -f $(OBJS) $(EXE)
	@rm -f tests/*.temp
	@rm -f tests_2/*.temp
	@rm -f jit_functions/*