#pragma once

#include <cstddef>

struct dive_token_t {
	size_t token_id;
	size_t begin;
	size_t end;
};

struct lexer_dfa_table_element_t {
	size_t next;
	size_t token_id;
};

class lexer_t {
	size_t current_row = 0;
	size_t current_stream_position = 0;

	size_t last_matched_token_id = -1;
	size_t last_match_begin = current_stream_position;
	size_t last_match_end;

public:
	dive_token_t push_character(char character) noexcept;
};
