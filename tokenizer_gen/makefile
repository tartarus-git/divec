CPP_COMPILER := clang++-16

CPP_COMP_PREAMBLE := $(CPP_COMPILER) --std=c++20 -Wall -Wextra

.PHONY: all build clean

all: build

build: bin/tokenizer_generator

OBJECTS_TO_LINK := bin/main.o \
bin/nfa_gen.o \
bin/dfa_gen.o

bin/tokenizer_generator: $(OBJECTS_TO_LINK)
	$(CPP_COMP_PREAMBLE) -o bin/tokenizer_generator $(OBJECTS_TO_LINK)

$(shell $(CPP_COMPILER) -MM main.cpp | aprepend --front bin/ > .make_temp_file)
include .make_temp_file
bin/main.o: bin/.dirstamp
	$(CPP_COMP_PREAMBLE) -o bin/main.o -c main.cpp

$(shell $(CPP_COMPILER) -MM nfa_gen.cpp | aprepend --front bin/ > .make_temp_file)
include .make_temp_file
bin/nfa_gen.o: bin/.dirstamp
	$(CPP_COMP_PREAMBLE) -o bin/nfa_gen.o -c nfa_gen.cpp

$(shell $(CPP_COMPILER) -MM dfa_gen.cpp | aprepend --front bin/ > .make_temp_file)
include .make_temp_file
bin/dfa_gen.o: bin/.dirstamp
	$(CPP_COMP_PREAMBLE) -o bin/dfa_gen.o -c dfa_gen.cpp

bin/.dirstamp:
	mkdir -p bin
	touch bin/.dirstamp

clean:
	git clean -fdx -e "*.swp"
