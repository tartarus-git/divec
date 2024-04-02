#include "lexer.h"

#include <cstdint>

// TODO: Remove this later.
#include <iostream>

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

	std::cout << current_row << '\n';

	if (row.token_id != -1) {
		current_token.id = row.token_id;
		current_token.end = current_stream_position;
	}

	const lexer_dfa_table_element_t &element = row.elements[character];


	if (element.next == 0) {
		std::cout << "token matched: " << current_token.id << '\n';

		current_row = 0;
		current_stream_position = current_token.end;

		last_token = current_token;

		current_token.id = -1;
		current_token.begin = current_token.end;

		return true;
	}

	current_row = element.next;
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

dive_token_t lexer_t::get_last_token() noexcept { return last_token; }
