#pragma once

#include <cstddef>

struct dive_token_t {
	size_t id;
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

	dive_token_t current_token {
		-1,
		current_stream_position,
		0
	};

	dive_token_t last_token {
		-1,
		0,
		0
	};

public:
	bool push_character(char character) noexcept;

	dive_token_t get_last_token() noexcept;
};
