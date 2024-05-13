#include "lexer.h"

#include <cstdlib>
#include <new>
#include <cstring>
#include <vector>

#include "helpers.h"

// TODO: remove this later
#include <iostream>

extern "C" {
	// These symbols are literally located at the start and end of the data.
	// They don't have extra storage that contains pointers to the start and end of the data.
	// They literally ARE the start and end of the data.
	// That's why we can't use classic pointer pattern here.
	extern lexer_dfa_table_row_t _binary_lexer_table_start;
	extern lexer_dfa_table_row_t _binary_lexer_table_end;
}

const lexer_dfa_table_row_t *lexer_table_begin = &_binary_lexer_table_start;
const lexer_dfa_table_row_t *lexer_table_end = &_binary_lexer_table_end;

bool lexer_t::push_inner(uint16_t character) noexcept {
	const lexer_dfa_table_row_t &row = lexer_table_begin[current_row];

	if (row.token_id != -1) {
		token_id = row.token_id;
		token_end = current_stream_position;
	}

	const size_t &edge = row.edges[character];

	if (edge == 0) {
		current_row = 0;
		current_stream_position = token_end;
		return true;
	}

	current_row = edge;
	current_stream_position++;

	return false;
}

bool lexer_t::push_character(char character) noexcept {
	static_assert(sizeof(char) == 1, "sizeof(char) must be 1 for lexer to work");

	return push_inner((uint16_t)character);
}

bool lexer_t::push_eof() noexcept {
	return push_inner(256);
}

dive_token_t lexer_t::get_last_token() noexcept {
	dive_token_type_t token_type;
	switch (token_id) {
	case (uint32_t)-1:
		token_type = dive_token_type_t::INVALID_TOKEN;
		break;
	default:
		token_type = (dive_token_type_t)token_id;
		break;
	}
	dive_token_t result { token_type, token_begin, token_end };

	token_id = -1;
	token_begin = token_end;

	return result;
}

divec_error_t diveLexProgram_inner(dive_program_t program, dive_build_log_t build_log) noexcept {

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

		if (match_found) {

			dive_token_t token = lexer.get_last_token();

			if (token.type == dive_token_type_t::INVALID_TOKEN) {

				if (build_log != nullptr) {
					dive_build_error_invalid_token_t *entry = (dive_build_error_invalid_token_t*)std::malloc(sizeof(dive_build_error_invalid_token_t));
					if (entry == nullptr) { return divec_error_t::OUT_OF_MEMORY; }

					const auto [source_code_line, source_code_column] = helpers::get_line_column_from_text_position(program->source_code, token.begin);
					if (source_code_line == -1 || source_code_column == -1) {
						// It's probably worthwhile not to touch heap state when bug happens for debugging purposes.
						// So we're not going to free entry here. Memory leak. The user should terminate the program anyway so it's fine.
						return divec_error_t::BUG;
					}

					divec_error_t err;
					new (entry) dive_build_error_invalid_token_t(source_code_line,
									       source_code_column,
									       program->source_code + token.begin,
									       (source_iter - program->source_code) + 1 - token.begin,
									       err);
					if (err != divec_error_t::SUCCESS) {
						std::free(entry);
						return err;
					}

					err = link_entry_to_build_log(build_log, entry);
					if (err != divec_error_t::SUCCESS) {
						std::free(entry);
						return err;
					}
				}

				return divec_error_t::BUILD_ERROR;
			}

			std::cout << (int)(token.type) << '\n';

			tokens.push_back(token);

			// If we've processed the EOF token, then stop processing.
			if (tokens.back().type == dive_token_type_t::EOF_TOKEN) { break; }

			source_iter = program->source_code + token.end;
			continue;
		}

		// keep this static if we're at the end, that way we can just keep pushing EOFs
		// onto the thing until we get an EOF token.
		// TODO: Think about more elegant ways to structure this function.
		if (*source_iter != '\0') { source_iter++; }
	}

	// pop EOF token off of vector, we don't need that one for parsing.
	tokens.pop_back();

	const size_t tokens_size = tokens.size() * sizeof(dive_token_t);

	program->tokens = (dive_token_t*)std::malloc(tokens_size);
	if (program->tokens == nullptr) { return divec_error_t::OUT_OF_MEMORY; }

	std::memcpy((dive_token_t*)program->tokens, tokens.data(), tokens_size);

	program->tokens_length = tokens.size();

	program->state = dive_program_state_t::LEXED;

	return divec_error_t::SUCCESS;

}
