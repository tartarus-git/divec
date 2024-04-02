#include "lexer.h"

#include <cstdint>

extern "C" {
	// These symbols are literally located at the start and end of the data.
	// They don't have extra storage that contains pointers to the start and end of the data.
	// They literally ARE the start and end of the data.
	// That's why we can't use classic pointer pattern here.
	extern char _binary_lexer_table_start;
	extern char _binary_lexer_table_end;
}

const lexer_dfa_table_row_t *lexer_table_begin = (lexer_dfa_table_row_t*)&_binary_lexer_table_start;
const lexer_dfa_table_row_t *lexer_table_end = (lexer_dfa_table_row_t*)&_binary_lexer_table_end;

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
