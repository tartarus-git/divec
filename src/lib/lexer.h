#pragma once

#include <cstddef>
#include <cstdint>

struct dive_token_t {
	size_t id;
	size_t begin;
	size_t end;
};

struct lexer_dfa_table_element_t {
	size_t next;
};

struct lexer_dfa_table_row_t {
	lexer_dfa_table_element_t elements[257];
	size_t token_id;
};

class lexer_t {
	size_t current_row = 0;
	size_t current_stream_position = 0;

	dive_token_t current_token {
		(size_t)-1,
		current_stream_position,
		0
	};

	dive_token_t last_token {
		(size_t)-1,
		0,
		0
	};

	bool push_inner(uint16_t character) noexcept;

public:
	bool push_character(char character) noexcept;

	bool push_eof() noexcept;

	dive_token_t get_last_token() noexcept;
};
