#pragma once

#include <cstdint>

#include "compiler_error.h"
#include "dive_token.h"

enum class dive_program_state_t : uint8_t {
	SOURCE,
	LEXED,
	PREPROCESSED,
	PARSED,
	DIVE_OPTIMIZED,
	TRANSLATED,
	BACKEND_OPTIMIZED,
	BACKEND_EMITTED
};

struct dive_program_t_inner {

	dive_program_state_t state;

	const char *source_code;

	const dive_token_t *tokens;
	size_t tokens_length;

};

using dive_program_t = dive_program_t_inner*;

dive_program_t diveCreateProgram_inner(const char *source_code) noexcept;

divec_error_t diveReleaseProgram_inner(dive_program_t program) noexcept;
