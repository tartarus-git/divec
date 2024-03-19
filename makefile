PLATFORM := LINUX_X64

COMPILER := clang++-16
OPTIMIZATION := -O0
WARNING_FLAGS := -Wall -Wextra
COMPILER_PREAMBLE := $(COMPILER) $(OPTIMIZATION) $(WARNING_FLAGS)

.PHONY: all build clean test

all: build

build: bin/divec

bin/divec: bin/src/*.o

$(shell $(COMPILER) -MM src/main.cpp | aprepend --front bin/src/ > .make_temp_file)
include .make_temp_file
bin/src/main.o: bin/src/.dirstamp
	$(COMPILER_PREAMBLE) -

# CUTOFF TO JUNK CODE FROM OTHER FILE

bin/lib/main/libblood.so: bin/lib/main/.dirstamp src/bindings/libblood.cpp
	$(COMPILER_PREAMBLE) -c src/bindings/libblood.cpp -o bin/lib/main/libblood.so

blood_engine_dll_sources := \
bin/lib/api.o \
bin/lib/context.o \
bin/lib/pipeline.o \
bin/lib/program.o

bin/lib/main/blood_engine.so: bin/lib/main/.dirstamp $(blood_engine_dll_sources)
	$(COMPILER_PREAMBLE) -shared -fvisibility=hidden $(blood_engine_dll_sources) -o bin/lib/main/blood_engine.so -pthread

$(shell $(COMPILER) -MM src/dll/api.cpp | aprepend --front bin/lib/ > .make_temp_file)
include .make_temp_file
bin/lib/api.o: bin/lib/.dirstamp
	$(COMPILER_PREAMBLE) -fPIC -c src/dll/api.cpp -o bin/lib/api.o

$(shell $(COMPILER) -MM src/dll/context.cpp | aprepend --front bin/lib/ > .make_temp_file)
include .make_temp_file
bin/lib/context.o: bin/lib/.dirstamp
	$(COMPILER_PREAMBLE) -fPIC -c src/dll/context.cpp -o bin/lib/context.o

$(shell $(COMPILER) -MM src/dll/pipeline.cpp | aprepend --front bin/lib/ > .make_temp_file)
include .make_temp_file
bin/lib/pipeline.o: bin/lib/.dirstamp
	$(COMPILER_PREAMBLE) -fPIC -c src/dll/pipeline.cpp -o bin/lib/pipeline.o

$(shell $(COMPILER) -MM src/dll/program.cpp | aprepend --front bin/lib/ > .make_temp_file)
include .make_temp_file
bin/lib/program.o: bin/lib/.dirstamp
	$(COMPILER_PREAMBLE) -fPIC -c src/dll/program.cpp -o bin/lib/program.o

bin/test: bin/test.o
	$(COMPILER_PREAMBLE) bin/test.o -o bin/test

$(shell $(COMPILER) -MM -I. test/main.cpp | aprepend --front bin/ > .make_temp_file)
include .make_temp_file
bin/test.o: bin/.dirstamp
	$(COMPILER_PREAMBLE) -I. -c test/main.cpp -o bin/test.o

bin/.dirstamp:
	mkdir -p bin
	touch bin/.dirstamp

bin/lib/.dirstamp: bin/.dirstamp
	mkdir -p bin/lib
	touch bin/lib/.dirstamp

bin/lib/main/.dirstamp: bin/.dirstamp bin/lib/.dirstamp
	mkdir -p bin/lib/main
	touch bin/lib/main/.dirstamp

clean:
	git clean -fdx -e "*.swp"
