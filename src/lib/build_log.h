#pragma once

#include "compiler_error.h"
#include "dive_token.h"

// leave at default bit-width for future-proofness,
// don't want to change API because could break user code
enum class dive_build_log_entry_type_t {
	NONE,
	ERROR_UNEXPECTED_TOKEN
};

struct dive_build_log_t_inner {
	void *entries;
};

using dive_build_log_t = dive_build_log_t_inner*;

#include "build_log_impl/unexpected_token.h"
// TODO: rest of them...

size_t diveGetBuildLogEntryStringSize_inner(dive_build_log_t build_log, void *entry) noexcept;

size_t diveGetBuildLogEntryString_inner(dive_build_log_t build_log,
					void *entry,
					char *buffer,
					size_t buffer_size,
					divec_error_t &err) noexcept;

size_t diveGetBuildLogStringSize_inner(dive_build_log_t build_log) noexcept;

size_t diveGetBuildLogString_inner(dive_build_log_t build_log,
				   char *buffer,
				   size_t buffer_size,
				   divec_error_t &err) noexcept;
