#pragma once

#include <cstddef>

#include "dive_token.h"

// IMPORTANT: We don't include build_log.h here because this header file isn't a normal header file.
// This will be included from build_log.h to create one big header file with all the different specializations.
// So dive_build_log_t and whatever we need will be available here, even if it doesn't look like it.

class dive_build_error_unexpected_token_t : public dive_build_log_entry_t_inner {
public:
	struct {
		size_t source_code_line;
		size_t source_code_column;

		const char *token_text;

		const dive_token_type_t *expected_token_types;
		size_t expected_token_types_length;
	} user_accessible;

	const char *string_representation = nullptr;
	size_t string_representation_size = 0;

	dive_build_error_unexpected_token_t(size_t source_code_line,
					    size_t source_code_column,
					    const char *token_text,
					    size_t token_text_length,
					    dive_token_type_t *expected_token_types,
					    size_t expected_token_types_length,
					    divec_error_t &err) noexcept;

private:
	divec_error_t gen_string_representation() noexcept;

public:
	void* get_user_accessible_data_ptr() noexcept override final;

	divec_error_t invalidate_string() noexcept override final;

	size_t get_string_size(divec_error_t &err) noexcept override final;
	size_t get_string(char *buffer, size_t buffer_size, divec_error_t &err) noexcept override final;

	divec_error_t free_children() noexcept override final;
};
