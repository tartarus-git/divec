#include "compiler.h"

#include <cstdlib>
#include <vector>
#include <cstring>

// TODO: Remove later.
#include <iostream>

dive_program_t diveCreateProgram_inner(const char *source_code) noexcept {

	dive_program_t result = (dive_program_t)std::malloc(sizeof(dive_program_t_inner));
	if (result == nullptr) { return nullptr; }

	const size_t source_code_size = std::strlen(source_code) * sizeof(char);

	char *source_code_copy = (char*)std::malloc(source_code_size);
	if (source_code_copy == nullptr) {
		std::free(result);
		return nullptr;
	}

	std::memcpy(source_code_copy, source_code, source_code_size);

	*result = {
		.state = dive_program_state_t::SOURCE,
		.source_code = source_code_copy,
		.tokens = nullptr,
		.dive_ast = nullptr
	};

	return result;

}

divec_error_t diveReleaseProgram_inner(dive_program_t program) noexcept {

	std::free((char*)program->source_code);
	std::free((dive_token_t*)program->tokens);
	std::free(program->dive_ast);

	std::free(program);

	return divec_error_t::SUCCESS;

}
