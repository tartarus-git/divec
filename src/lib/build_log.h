#pragma once

#include "compiler_error.h"
#include "dive_token.h"

// leave at default bit-width for future-proofness,
// don't want to change API because could break user code
enum class dive_build_log_entry_type_t {
	NULL_ENTRY,
	ERROR_INVALID_TOKEN,
	ERROR_UNEXPECTED_TOKEN
};

class dive_build_log_entry_t_inner;
using dive_build_log_entry_t = dive_build_log_entry_t_inner*;

class dive_build_log_entry_t_inner {
public:
	dive_build_log_entry_type_t entry_type = dive_build_log_entry_type_t::NULL_ENTRY;

	dive_build_log_entry_t next = nullptr;

	dive_build_log_entry_t_inner(dive_build_log_entry_type_t entry_type) noexcept;

	virtual void* get_user_accessible_data_ptr() noexcept = 0;

	virtual size_t get_string_size(divec_error_t &err) noexcept = 0;
	virtual size_t get_string(char *buffer,
				  size_t buffer_size,
				  divec_error_t &err) noexcept = 0;

	divec_error_t base_free_children() noexcept;
	virtual divec_error_t free_children() noexcept = 0;
};

struct dive_build_log_t_inner {
	dive_build_log_entry_t entries;
	dive_build_log_entry_t last_entry;
};

using dive_build_log_t = dive_build_log_t_inner*;

#include "build_log_impl/unexpected_token.h"
#include "build_log_impl/invalid_token.h"

// USER-FACING FUNCTIONS

dive_build_log_t diveCreateBuildLog_inner(divec_error_t &err) noexcept;

dive_build_log_entry_t diveGetNextBuildLogEntry_inner(dive_build_log_entry_t entry, divec_error_t &err) noexcept;

dive_build_log_entry_type_t diveGetBuildLogEntryType_inner(dive_build_log_entry_t entry, divec_error_t &err) noexcept;

void* diveGetBuildLogEntryContents_inner(dive_build_log_entry_t entry, divec_error_t &err) noexcept;

size_t diveGetBuildLogEntryStringSize_inner(dive_build_log_entry_t entry, divec_error_t &err) noexcept;

size_t diveGetBuildLogEntryString_inner(dive_build_log_entry_t entry,
					char *buffer,
					size_t buffer_size,
					divec_error_t &err) noexcept;

size_t diveGetBuildLogStringSize_inner(dive_build_log_t build_log, divec_error_t &err) noexcept;

size_t diveGetBuildLogString_inner(dive_build_log_t build_log,
				   char *buffer,
				   size_t buffer_size,
				   divec_error_t &err) noexcept;

divec_error_t diveReleaseBuildLog_inner(dive_build_log_t build_log) noexcept;

// BACKEND FUNCTIONS

divec_error_t link_entry_to_build_log(dive_build_log_t build_log, dive_build_log_entry_t entry) noexcept;
