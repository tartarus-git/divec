PLATFORM := LINUX_X64

COMPILER := clang++-16
OPTIMIZATION := -O0
WARNING_FLAGS := -Wall -Wextra
COMPILER_PREAMBLE := $(COMPILER) $(OPTIMIZATION) $(WARNING_FLAGS)

.PHONY: all build lexer_generator test clean

all: build test

# ----- libdivec BEGIN -----

build: bin/libdivec.so

libdivec_objects := \
bin/libdivec_build/lexer.o

bin/libdivec.so: bin/.dirstamp $(libdivec_objects)
	$(COMPILER_PREAMBLE) -shared -fvisibility=hidden $(libdivec_objects) -o bin/libdivec.so

$(shell $(COMPILER) -MM -MG -I. src/lib/lexer.cpp | aprepend --front bin/libdivec_build/ > .make_temp_file)
include .make_temp_file
bin/libdivec_build/lexer.o: bin/libdivec_build/.dirstamp
	$(COMPILER_PREAMBLE) -fPIC -I. -c src/lib/lexer.cpp -o bin/libdivec_build/lexer.o

embed/lexer_table.h: lexer_generator embed/lexer.lexer_spec
	./tokenizer_gen/bin/tokenizer_generator
	cat embed/.temp_lexer_table | srcembed --varname raw_lexer_table c++ | aprepend --front -b 10 | aprepend --front '#pragma once' > embed/lexer_table.h

lexer_generator:
	cd tokenizer_gen && $(MAKE)

# ----- libdivec END -----

# ----- test BEGIN -----

test: bin/test

test_objects := \
bin/test_build/main.o

bin/test: bin/.dirstamp $(test_objects)
	$(COMPILER_PREAMBLE) $(test_objects) -o bin/test

# ----- test END -----

bin/libdivec_build/.dirstamp: bin/.dirstamp
	mkdir -p bin/libdivec_build
	touch bin/libdivec_build/.dirstamp

bin/test_build/.dirstamp: bin/.dirstamp
	mkdir -p bin/test_build
	touch bin/test_build/.dirstamp

bin/.dirstamp:
	mkdir -p bin
	touch bin/.dirstamp

clean:
	git clean -fdx -e '*.swp'
