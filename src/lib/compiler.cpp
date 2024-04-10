#include "compiler.h"

#include "lexer.h"

// TODO: remove later
#include <iostream>

divec_error_t diveCompileProgram_inner(dive_program_t program, dive_build_log_t build_log) noexcept {

	divec_error_t err = diveLexProgram_inner(program, build_log);
	if (err != divec_error_t::SUCCESS) { return err; }

	for (size_t i = 0; i < program->tokens_length; i++) {
		std::cout << (int)(program->tokens[i].type) << "\n";
	}

	return divec_error_t::SUCCESS;

}
