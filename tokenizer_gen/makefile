CPP_COMPILER := clang++-16

CPP_COMP_PREAMBLE := $(CPP_COMPILER) -Wall -Wextra

.PHONY: all build clean

all: build

build: bin/tokenizer_generator

bin/tokenizer_generator: bin/main.o bin/nfa_gen.o
	$(CPP_COMP_PREAMBLE) -o bin/tokenizer_generator bin/main.o bin/nfa_gen.o

$(shell $(CPP_COMPILER) -MM main.cpp | aprepend --front bin/ > .make_temp_file)
include .make_temp_file
bin/main.o: bin/.dirstamp
	$(CPP_COMP_PREAMBLE) -o bin/main.o -c main.cpp

$(shell $(CPP_COMPILER) -MM nfa_gen.cpp | aprepend --front bin/ > .make_temp_file)
include .make_temp_file
bin/nfa_gen.o: bin/.dirstamp
	$(CPP_COMP_PREAMBLE) -o bin/nfa_gen.o -c nfa_gen.cpp

bin/.dirstamp:
	mkdir -p bin
	touch bin/.dirstamp

clean:
	git clean -fdx -e "*.swp"
