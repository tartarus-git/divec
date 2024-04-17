PLATFORM := LINUX_X64

COMPILER := clang++-16
OPTIMIZATION := -O0
WARNING_FLAGS := -Wall -Wextra

HEADER_FINDER_PREAMBLE := $(COMPILER) -Isrc/lib

# TODO: Separate preambles so that the test build doesn't include src/lib headers.
COMPILER_PREAMBLE := $(COMPILER) --std=c++20 $(OPTIMIZATION) $(WARNING_FLAGS) -Isrc/lib

EMIT_ASSEMBLY := 0
ifeq ($(EMIT_ASSEMBLY),1)
EMIT_ASSEMBLY_PREAMBLE := $(COMPILER_PREAMBLE) -fverbose-asm -S
else
EMIT_ASSEMBLY_PREAMBLE := true
endif

.PHONY: all build test clean

all: build test

# ----- libdivec BEGIN -----

build: bin/libdivec.so

libdivec_so_objects := \
bin/libdivec_so_build/api.o \
bin/libdivec_so_build/program.o \
bin/libdivec_so_build/compiler.o \
bin/libdivec_so_build/dive_token.o \
bin/libdivec_so_build/lexer.o \
bin/libdivec_so_build/lexer_table.o \
bin/libdivec_so_build/preprocessor.o \
bin/libdivec_so_build/dive_ast_impl/program.o \
bin/libdivec_so_build/dive_ast_impl/function.o \
bin/libdivec_so_build/dive_ast_impl/statement.o \
bin/libdivec_so_build/parser.o \
bin/libdivec_so_build/parser_impl/type.o \
bin/libdivec_so_build/parser_impl/identifier.o \
bin/libdivec_so_build/parser_impl/program.o \
bin/libdivec_so_build/parser_impl/function.o \
bin/libdivec_so_build/parser_impl/statement.o \
bin/libdivec_so_build/build_log.o \
bin/libdivec_so_build/build_log_impl/invalid_token.o \
bin/libdivec_so_build/build_log_impl/unexpected_token.o \
bin/libdivec_so_build/helpers.o

bin/libdivec.so: bin/.dirstamp $(libdivec_so_objects)
	$(COMPILER_PREAMBLE) -shared -fvisibility=hidden $(libdivec_so_objects) -o bin/libdivec.so

$(shell $(HEADER_FINDER_PREAMBLE) -MM src/lib/api.cpp | aprepend --front bin/libdivec_so_build/ > .make_temp_file)
include .make_temp_file
bin/libdivec_so_build/api.o: bin/libdivec_so_build/.dirstamp
	$(EMIT_ASSEMBLY_PREAMBLE) src/lib/api.cpp -o bin/libdivec_so_build/api.asm
	$(COMPILER_PREAMBLE) -fPIC -c src/lib/api.cpp -o bin/libdivec_so_build/api.o

$(shell $(HEADER_FINDER_PREAMBLE) -MM src/lib/program.cpp | aprepend --front bin/libdivec_so_build/ > .make_temp_file)
include .make_temp_file
bin/libdivec_so_build/program.o: bin/libdivec_so_build/.dirstamp
	$(EMIT_ASSEMBLY_PREAMBLE) src/lib/program.cpp -o bin/libdivec_so_build/program.asm
	$(COMPILER_PREAMBLE) -fPIC -c src/lib/program.cpp -o bin/libdivec_so_build/program.o

$(shell $(HEADER_FINDER_PREAMBLE) -MM src/lib/compiler.cpp | aprepend --front bin/libdivec_so_build/ > .make_temp_file)
include .make_temp_file
bin/libdivec_so_build/compiler.o: bin/libdivec_so_build/.dirstamp
	$(EMIT_ASSEMBLY_PREAMBLE) src/lib/compiler.cpp -o bin/libdivec_so_build/compiler.asm
	$(COMPILER_PREAMBLE) -fPIC -c src/lib/compiler.cpp -o bin/libdivec_so_build/compiler.o

# ----- lexer BEGIN -----

$(shell $(HEADER_FINDER_PREAMBLE) -MM src/lib/dive_token.cpp | aprepend --front bin/libdivec_so_build/ > .make_temp_file)
include .make_temp_file
bin/libdivec_so_build/dive_token.o: bin/libdivec_so_build/.dirstamp
	$(EMIT_ASSEMBLY_PREAMBLE) src/lib/dive_token.cpp -o bin/libdivec_so_build/dive_token.asm
	$(COMPILER_PREAMBLE) -fPIC -c src/lib/dive_token.cpp -o bin/libdivec_so_build/dive_token.o

$(shell $(HEADER_FINDER_PREAMBLE) -MM src/lib/lexer.cpp | aprepend --front bin/libdivec_so_build/ > .make_temp_file)
include .make_temp_file
bin/libdivec_so_build/lexer.o: bin/libdivec_so_build/.dirstamp
	$(EMIT_ASSEMBLY_PREAMBLE) src/lib/lexer.cpp -o bin/libdivec_so_build/lexer.asm
	$(COMPILER_PREAMBLE) -fPIC -c src/lib/lexer.cpp -o bin/libdivec_so_build/lexer.o

bin/libdivec_so_build/lexer_table.o: bin/libdivec_so_build/.dirstamp build_intermediates/.dirstamp tokenizer_gen/bin/tokenizer_generator lexer.lexer_spec
	./tokenizer_gen/bin/tokenizer_generator
	cp .temp_lexer_table build_intermediates/lexer_table
	cd build_intermediates && ld -z noexecstack -r -b binary lexer_table -o ../bin/libdivec_so_build/lexer_table.o

tokenizer_gen/bin/tokenizer_generator:
	cd tokenizer_gen && $(MAKE)

# ----- lexer END -----

# ----- preprocessor BEGIN -----

$(shell $(HEADER_FINDER_PREAMBLE) -MM src/lib/preprocessor.cpp | aprepend --front bin/libdivec_so_build/ > .make_temp_file)
include .make_temp_file
bin/libdivec_so_build/preprocessor.o: bin/libdivec_so_build/.dirstamp
	$(EMIT_ASSEMBLY_PREAMBLE) src/lib/preprocessor.cpp -o bin/libdivec_so_build/preprocessor.asm
	$(COMPILER_PREAMBLE) -fPIC -c src/lib/preprocessor.cpp -o bin/libdivec_so_build/preprocessor.o

# ----- preprocessor END -----

# ----- dive_ast BEGIN -----

$(shell $(HEADER_FINDER_PREAMBLE) -MM src/lib/dive_ast_impl/program.cpp | aprepend --front bin/libdivec_so_build/dive_ast_impl/ > .make_temp_file)
include .make_temp_file
bin/libdivec_so_build/dive_ast_impl/program.o: bin/libdivec_so_build/dive_ast_impl/.dirstamp
	$(EMIT_ASSEMBLY_PREAMBLE) src/lib/dive_ast_impl/program.cpp -o bin/libdivec_so_build/dive_ast_impl/program.asm
	$(COMPILER_PREAMBLE) -fPIC -c src/lib/dive_ast_impl/program.cpp -o bin/libdivec_so_build/dive_ast_impl/program.o

$(shell $(HEADER_FINDER_PREAMBLE) -MM src/lib/dive_ast_impl/function.cpp | aprepend --front bin/libdivec_so_build/dive_ast_impl/ > .make_temp_file)
include .make_temp_file
bin/libdivec_so_build/dive_ast_impl/function.o: bin/libdivec_so_build/dive_ast_impl/.dirstamp
	$(EMIT_ASSEMBLY_PREAMBLE) src/lib/dive_ast_impl/function.cpp -o bin/libdivec_so_build/dive_ast_impl/function.asm
	$(COMPILER_PREAMBLE) -fPIC -c src/lib/dive_ast_impl/function.cpp -o bin/libdivec_so_build/dive_ast_impl/function.o

$(shell $(HEADER_FINDER_PREAMBLE) -MM src/lib/dive_ast_impl/statement.cpp | aprepend --front bin/libdivec_so_build/dive_ast_impl/ > .make_temp_file)
include .make_temp_file
bin/libdivec_so_build/dive_ast_impl/statement.o: bin/libdivec_so_build/dive_ast_impl/.dirstamp
	$(EMIT_ASSEMBLY_PREAMBLE) src/lib/dive_ast_impl/statement.cpp -o bin/libdivec_so_build/dive_ast_impl/statement.asm
	$(COMPILER_PREAMBLE) -fPIC -c src/lib/dive_ast_impl/statement.cpp -o bin/libdivec_so_build/dive_ast_impl/statement.o

# ----- dive_ast END -----

# ----- parser BEGIN -----

$(shell $(HEADER_FINDER_PREAMBLE) -MM src/lib/parser.cpp | aprepend --front bin/libdivec_so_build/ > .make_temp_file)
include .make_temp_file
bin/libdivec_so_build/parser.o: bin/libdivec_so_build/.dirstamp
	$(EMIT_ASSEMBLY_PREAMBLE) src/lib/parser.cpp -o bin/libdivec_so_build/parser.asm
	$(COMPILER_PREAMBLE) -fPIC -c src/lib/parser.cpp -o bin/libdivec_so_build/parser.o

$(shell $(HEADER_FINDER_PREAMBLE) -MM src/lib/parser_impl/type.cpp | aprepend --front bin/libdivec_so_build/parser_impl/ > .make_temp_file)
include .make_temp_file
bin/libdivec_so_build/parser_impl/type.o: bin/libdivec_so_build/parser_impl/.dirstamp
	$(EMIT_ASSEMBLY_PREAMBLE) src/lib/parser_impl/type.cpp -o bin/libdivec_so_build/parser_impl/type.asm
	$(COMPILER_PREAMBLE) -fPIC -c src/lib/parser_impl/type.cpp -o bin/libdivec_so_build/parser_impl/type.o

$(shell $(HEADER_FINDER_PREAMBLE) -MM src/lib/parser_impl/identifier.cpp | aprepend --front bin/libdivec_so_build/parser_impl/ > .make_temp_file)
include .make_temp_file
bin/libdivec_so_build/parser_impl/identifier.o: bin/libdivec_so_build/parser_impl/.dirstamp
	$(EMIT_ASSEMBLY_PREAMBLE) src/lib/parser_impl/identifier.cpp -o bin/libdivec_so_build/parser_impl/identifier.asm
	$(COMPILER_PREAMBLE) -fPIC -c src/lib/parser_impl/identifier.cpp -o bin/libdivec_so_build/parser_impl/identifier.o

$(shell $(HEADER_FINDER_PREAMBLE) -MM src/lib/parser_impl/program.cpp | aprepend --front bin/libdivec_so_build/parser_impl/ > .make_temp_file)
include .make_temp_file
bin/libdivec_so_build/parser_impl/program.o: bin/libdivec_so_build/parser_impl/.dirstamp
	$(EMIT_ASSEMBLY_PREAMBLE) src/lib/parser_impl/program.cpp -o bin/libdivec_so_build/parser_impl/program.asm
	$(COMPILER_PREAMBLE) -fPIC -c src/lib/parser_impl/program.cpp -o bin/libdivec_so_build/parser_impl/program.o

$(shell $(HEADER_FINDER_PREAMBLE) -MM src/lib/parser_impl/function.cpp | aprepend --front bin/libdivec_so_build/parser_impl/ > .make_temp_file)
include .make_temp_file
bin/libdivec_so_build/parser_impl/function.o: bin/libdivec_so_build/parser_impl/.dirstamp
	$(EMIT_ASSEMBLY_PREAMBLE) src/lib/parser_impl/function.cpp -o bin/libdivec_so_build/parser_impl/function.asm
	$(COMPILER_PREAMBLE) -fPIC -c src/lib/parser_impl/function.cpp -o bin/libdivec_so_build/parser_impl/function.o

$(shell $(HEADER_FINDER_PREAMBLE) -MM src/lib/parser_impl/statement.cpp | aprepend --front bin/libdivec_so_build/parser_impl/ > .make_temp_file)
include .make_temp_file
bin/libdivec_so_build/parser_impl/statement.o: bin/libdivec_so_build/parser_impl/.dirstamp
	$(EMIT_ASSEMBLY_PREAMBLE) src/lib/parser_impl/statement.cpp -o bin/libdivec_so_build/parser_impl/statement.asm
	$(COMPILER_PREAMBLE) -fPIC -c src/lib/parser_impl/statement.cpp -o bin/libdivec_so_build/parser_impl/statement.o

# ----- parser END -----

# ----- build log START -----

$(shell $(HEADER_FINDER_PREAMBLE) -MM src/lib/build_log.cpp | aprepend --front bin/libdivec_so_build/ > .make_temp_file)
include .make_temp_file
bin/libdivec_so_build/build_log.o: bin/libdivec_so_build/.dirstamp
	$(EMIT_ASSEMBLY_PREAMBLE) src/lib/build_log.cpp -o bin/libdivec_so_build/build_log.asm
	$(COMPILER_PREAMBLE) -fPIC -c src/lib/build_log.cpp -o bin/libdivec_so_build/build_log.o

$(shell $(HEADER_FINDER_PREAMBLE) -MM src/lib/build_log_impl/invalid_token.cpp | aprepend --front bin/libdivec_so_build/ > .make_temp_file)
include .make_temp_file
bin/libdivec_so_build/build_log_impl/invalid_token.o: bin/libdivec_so_build/build_log_impl/.dirstamp
	$(EMIT_ASSEMBLY_PREAMBLE) src/lib/build_log_impl/invalid_token.cpp -o bin/libdivec_so_build/build_log_impl/invalid_token.asm
	$(COMPILER_PREAMBLE) -fPIC -c src/lib/build_log_impl/invalid_token.cpp -o bin/libdivec_so_build/build_log_impl/invalid_token.o

$(shell $(HEADER_FINDER_PREAMBLE) -MM src/lib/build_log_impl/unexpected_token.cpp | aprepend --front bin/libdivec_so_build/ > .make_temp_file)
include .make_temp_file
bin/libdivec_so_build/build_log_impl/unexpected_token.o: bin/libdivec_so_build/build_log_impl/.dirstamp
	$(EMIT_ASSEMBLY_PREAMBLE) src/lib/build_log_impl/unexpected_token.cpp -o bin/libdivec_so_build/build_log_impl/unexpected_token.asm
	$(COMPILER_PREAMBLE) -fPIC -c src/lib/build_log_impl/unexpected_token.cpp -o bin/libdivec_so_build/build_log_impl/unexpected_token.o

# ----- build log END -----

$(shell $(HEADER_FINDER_PREAMBLE) -MM src/lib/helpers.cpp | aprepend --front bin/libdivec_so_build/ > .make_temp_file)
include .make_temp_file
bin/libdivec_so_build/helpers.o: bin/libdivec_so_build/.dirstamp
	$(EMIT_ASSEMBLY_PREAMBLE) src/lib/helpers.cpp -o bin/libdivec_so_build/helpers.asm
	$(COMPILER_PREAMBLE) -fPIC -c src/lib/helpers.cpp -o bin/libdivec_so_build/helpers.o

# ----- libdivec END -----

# ----- test BEGIN -----

test: bin/test

test_objects := \
bin/test_build/main.o

bin/test: bin/.dirstamp $(test_objects) bin/libdivec.so
	$(COMPILER_PREAMBLE) $(test_objects) -o bin/test -Lbin -Wl,-rpath,'$$ORIGIN' -ldivec

$(shell $(HEADER_FINDER_PREAMBLE) -MM -Iinclude test/main.cpp | aprepend --front bin/test_build/ > .make_temp_file)
include .make_temp_file
bin/test_build/main.o: bin/test_build/.dirstamp
	$(EMIT_ASSEMBLY_PREAMBLE) -Iinclude test/main.cpp -o bin/test_build/main.asm
	$(COMPILER_PREAMBLE) -Iinclude -c test/main.cpp -o bin/test_build/main.o

# ----- test END -----

bin/libdivec_so_build/.dirstamp: bin/.dirstamp
	mkdir -p bin/libdivec_so_build
	touch bin/libdivec_so_build/.dirstamp

bin/libdivec_so_build/dive_ast_impl/.dirstamp: bin/libdivec_so_build/.dirstamp
	mkdir -p bin/libdivec_so_build/dive_ast_impl
	touch bin/libdivec_so_build/dive_ast_impl/.dirstamp

bin/libdivec_so_build/parser_impl/.dirstamp: bin/libdivec_so_build/.dirstamp
	mkdir -p bin/libdivec_so_build/parser_impl
	touch bin/libdivec_so_build/parser_impl/.dirstamp

bin/libdivec_so_build/build_log_impl/.dirstamp: bin/libdivec_so_build/.dirstamp
	mkdir -p bin/libdivec_so_build/build_log_impl
	touch bin/libdivec_so_build/build_log_impl/.dirstamp

bin/test_build/.dirstamp: bin/.dirstamp
	mkdir -p bin/test_build
	touch bin/test_build/.dirstamp

bin/.dirstamp:
	mkdir -p bin
	touch bin/.dirstamp

build_intermediates/.dirstamp:
	mkdir -p build_intermediates
	touch build_intermediates/.dirstamp

clean:
	git clean -fdx -e '*.swp'
