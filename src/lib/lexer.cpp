#include "lexer.h"

#include <cstdint>

extern "C" {
	extern char *_binary_lexer_table_start;
	extern char *_binary_lexer_table_end;
}

const lexer_dfa_table_element_t *lexer_table_begin = (lexer_dfa_table_element_t*)_binary_lexer_table_start;
const lexer_dfa_table_element_t *lexer_table_end = (lexer_dfa_table_element_t*)_binary_lexer_table_end;

bool lexer_t::push_character(char character) noexcept {
	static_assert(sizeof(char) == 1, "sizeof(char) must be 1 for lexer to work");

	const lexer_dfa_table_element_t &element = lexer_table_begin[current_row * 256 + (uint8_t)character];

	if (element.next == 0) {
		current_row = 0;
		current_stream_position = current_token.end;

		last_token = current_token;

		current_token.id = -1;
		current_token.begin = current_token.end;

		return true;
	}

	current_row = element.next;
	current_stream_position++;

	if (element.token_id != -1) {
		current_token.id = element.token_id;
		current_token.end = current_stream_position;
	}

	return false;
}

dive_token_t lexer_t::get_last_token() noexcept { return last_token; }
