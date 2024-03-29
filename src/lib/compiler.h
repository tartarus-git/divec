#pragma once

#include <cstdint>

enum class dive_program_state_t : uint8_t {
	SOURCE,
	LEXED,
	PARSED,
	DIVE_AST_OPTIMIZED,
	TRANSLATED,
	BACKEND_AST_OPTIMIZED,
	BACKEND_EMITTED
};

struct dive_program_t_inner {

	dive_program_state_t state;

	const char *source_code;

	const dive_token_t *tokens;

};

using dive_program_t = dive_program_t_inner*;

dive_program_t diveCreateProgram_inner(const char *source_code) noexcept;
