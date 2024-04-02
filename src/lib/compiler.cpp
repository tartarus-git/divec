#include "compiler.h"

#include <cstdlib>
#include <vector>
#include <cstring>

#include "lexer.h"

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
		.tokens = nullptr
	};

	return result;

}

divec_error_t lex_program(dive_program_t program, dive_build_log_t build_log) noexcept {

	switch (program->state) {
	case dive_program_state_t::SOURCE: break;
	default: return divec_error_t::ALREADY_DONE;
	}

	const char *source_iter = program->source_code;

	// TODO: Create your own vector implementation that allows to catch the out of memory case without
	// exceptions. Not that the standard version even allows you to do it with exceptions. It's bad.
	// Documentation is bad. C++ std is bad in general.
	// Also your own custom one could have orphan functionality, where it lets you take ownership over it's
	// memory before killing itself.
	std::vector<dive_token_t> tokens;

	lexer_t lexer;

	while (true) {
		bool match_found;
		if (*source_iter == '\0') {
			match_found = lexer.push_eof();
		} else {
			match_found = lexer.push_character(*source_iter);
		}

		std::cout << "character: " << *source_iter << '\n';

		if (match_found) {

			dive_token_t token = lexer.get_last_token();

			if (token.id == -1) {

				if (build_log != nullptr) {
					// TODO: Put error in build log.
				}

				std::cerr << "last end: " << tokens.end()->end << '\n';

				return divec_error_t::BUILD_ERROR;
			}

			tokens.push_back(token);

			// If we've processed the EOF token, then stop processing.
			if (tokens.back().id == 0) { break; }

			source_iter = program->source_code + token.end;
			continue;
		}

		if (*source_iter != '\0') { source_iter++; }
	}

	// pop EOF token off of vector, we don't need that one for parsing.
	tokens.pop_back();

	const size_t tokens_size = tokens.size() * sizeof(dive_token_t);

	program->tokens = (dive_token_t*)std::malloc(tokens_size);
	if (program->tokens == nullptr) { return divec_error_t::OUT_OF_MEMORY; }

	std::memcpy((dive_token_t*)program->tokens, tokens.data(), tokens_size);

	program->state = dive_program_state_t::LEXED;

	return divec_error_t::SUCCESS;

}

divec_error_t diveCompileProgram_inner(dive_program_t program, dive_build_log_t build_log) noexcept {

	divec_error_t err = lex_program(program, build_log);
	if (err != divec_error_t::SUCCESS) { return err; }

	return divec_error_t::SUCCESS;

}

divec_error_t diveReleaseProgram_inner(dive_program_t program) noexcept {

	std::free((char*)program->source_code);
	std::free((dive_token_t*)program->tokens);

	std::free(program);

	return divec_error_t::SUCCESS;

}
