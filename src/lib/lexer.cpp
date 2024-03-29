#include "lexer.h"

#include <cstdint>

extern "C" {
	extern char *_binary_lexer_table_start;
	extern char *_binary_lexer_table_end;
}

const lexer_dfa_table_element_t *lexer_table_begin = (lexer_dfa_table_element_t*)_binary_lexer_table_start;
const lexer_dfa_table_element_t *lexer_table_end = (lexer_dfa_table_element_t*)_binary_lexer_table_end;

lexer_token_t lexer_t::push_character(char character) noexcept {
	static_assert(sizeof(char) == 1, "sizeof(char) must be 1 for lexer to work");

	const lexer_dfa_table_element_t &element = lexer_table_begin[current_row * 256 + (uint8_t)character];

	if (element.next == 0) {
		current_row = 0;
		current_stream_position = last_match_end;

		const size_t token_id = last_matched_token_id;
		const size_t match_begin = last_match_begin;
		const size_t match_end = last_match_end;

		last_matched_token_id = -1;
		last_match_begin = current_stream_position;

		return { token_id, match_begin, match_end };
	}

	current_row = element.next;
	current_stream_position++;

	if (element.token_id != -1) {
		last_matched_token_id = element.token_id;
		last_match_end = current_stream_position;
	}
}
