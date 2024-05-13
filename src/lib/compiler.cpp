#include "compiler.h"

#include <cstdlib>
#include <cstring>

#include "lexer.h"
#include "preprocessor.h"
#include "parser.h"

// TODO: remove later
#include <iostream>

// TODO: Either remove later or put inside #define switch.
#include "debug.h"

divec_error_t diveCompileProgram_inner(dive_program_t program, dive_build_log_t build_log) noexcept {

	divec_error_t err = diveLexProgram_inner(program, build_log);
	if (err != divec_error_t::SUCCESS) { return err; }

	for (size_t i = 0; i < program->tokens_length; i++) {
		std::cout << (int)(program->tokens[i].type) << "\n";
	}

	// TODO: preprocess program

	/*dive_token_t temp_token_buffer[] {
		{ dive_token_type_t::INT32 },
		{ dive_token_type_t::IDENTIFIER },
		{ dive_token_type_t::OPENING_PARENTHESIS },
		{ dive_token_type_t::INT32 },
		{ dive_token_type_t::IDENTIFIER },
		//{ dive_token_type_t::INT32 },
		{ dive_token_type_t::CLOSING_PARENTHESIS },
		{ dive_token_type_t::OPENING_ANGEL_BRACKET },
		{ dive_token_type_t::IDENTIFIER },
		{ dive_token_type_t::CLOSING_ANGEL_BRACKET },
		{ dive_token_type_t::EOF_TOKEN }
	};

	dive_token_t *heap_token_array = (dive_token_t*)std::malloc(sizeof(temp_token_buffer));
	if (heap_token_array == nullptr) { return divec_error_t::OUT_OF_MEMORY; }

	std::memcpy(heap_token_array, temp_token_buffer, sizeof(temp_token_buffer));

	program->state = dive_program_state_t::PREPROCESSED;
	program->tokens = heap_token_array;
	program->tokens_length = sizeof(temp_token_buffer) / sizeof(dive_token_t);

	std::cout << "Got to being finished with the preparation.\n";*/

	err = diveParseProgram_inner(program, build_log);
	if (err != divec_error_t::SUCCESS) { return err; }

	debug::print_dive_ast_program(*(program->dive_ast));

	return divec_error_t::SUCCESS;

}
