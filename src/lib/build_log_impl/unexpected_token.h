#pragma once

#include <cstddef>

#include "compiler_error.h"
#include "dive_token.h"

// IMPORTANT: We don't include build_log.h here because this header file isn't a normal header file.
// This will be included from build_log.h to create one big header file with all the different specializations.
// So dive_build_log_t and whatever we need will be available here, even if it doesn't look like it.

struct dive_build_error_invalid_token_t {
	dive_build_log_entry_type entry_type;

	size_t source_code_line;
	size_t source_code_column;
	dive_token_type_t token_type;
	dive_token_type_t *expected_token_types;

	const char *string_representation;

	dive_build_error_invalid_token_t() noexcept;

private:
	divec_error_t gen_string_representation(dive_build_log_t build_log) noexcept;

public:
	size_t get_string_size(dive_build_log_t build_log) noexcept;
	size_t get_string(dive_build_log_t build_log, char *buffer, size_t buffer_size, divec_error_t &err) noexcept;
};
