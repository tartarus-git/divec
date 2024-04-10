#pragma once

#include <cstddef>
#include <cstdint>

#include "compiler_error.h"
#include "build_log.h"
#include "dive_token.h"
#include "program.h"

struct lexer_dfa_table_row_t {
	size_t edges[257];
	uint32_t token_id;
};

class lexer_t {
	size_t current_row = 0;
	size_t current_stream_position = 0;

	uint32_t token_id = -1;
	size_t token_begin = current_stream_position;
	size_t token_end = token_begin;

	// character MUST be in range 0-256 inclusive on both sides
	bool push_inner(uint16_t character) noexcept;

public:
	bool push_character(char character) noexcept;

	bool push_eof() noexcept;

	dive_token_t get_last_token() noexcept;
};

divec_error_t diveLexProgram_inner(dive_program_t program, dive_build_log_t build_log) noexcept;
