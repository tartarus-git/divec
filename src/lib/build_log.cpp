#include "build_log.h"

#include <cstring>

size_t diveGetBuildLogEntryStringSize_inner(dive_build_log_t build_log, void *entry) noexcept {

	// Necessary to do it like this because of strict aliasing rules in C++.
	// This is the only way to avoid UB when trying to do this.
	dive_build_log_entry_type_t entry_type;
	std::memcpy(&entry_type, entry, sizeof(entry_type));

	switch (entry_type) {
	case dive_build_log_entry_type_t::ERROR_UNEXPECTED_TOKEN:
		return ((dive_build_error_unexpected_token_t*)entry)->get_string_size(build_log, entry);
	default:
		return -1;
		// TODO: Make this throw back an error or something.
	}

}

size_t diveGetBuildLogEntryString_inner(dive_build_log_t build_log,
					void *entry,
					char *buffer,
					size_t buffer_size,
					divec_error_t &err) noexcept
{

	err = divec_error_t::SUCCESS;

	// Same note as above.
	dive_build_log_entry_type_t entry_type;
	std::memcpy(&entry_type, entry, sizeof(entry_type));

	switch (entry_type) {
	case dive_build_log_entry_type_t::ERROR_UNEXPECTED_TOKEN:
		return ((dive_build_error_unexpected_token_t*)entry)->get_string(build_log, entry, buffer, buffer_size, err);
	default:
		err = divec_error_t::INVALID_ARGUMENT;
		return -1;
	}

}

size_t diveGetBuildLogStringSize_inner(dive_build_log_t build_log) noexcept {

	size_t sum = 0;

	for (void *entry = build_log->entries; entry != nullptr; entry = entry->next) {
		sum += diveGetBuildLogEntryStringSize_inner(build_log, entry);
	}

	return sum;

}

size_t diveGetBuildLogString_inner(dive_build_log_t build_log,
				   char *buffer,
				   size_t buffer_size,
				   divec_error_t &err) noexcept
{

	err = divec_error_t::SUCCESS;

	size_t original_buffer_size = buffer_size;

	for (void *entry = build_log->entries; entry != nullptr; entry = entry->next) {

		size_t bytes_written = diveGetBuildLogEntryString_inner(build_log, entry, buffer, buffer_size, err);
		if (err != divec_error_t::SUCCESS) { return -1; }

		buffer += bytes_written;
		buffer_size -= bytes_written;

	}

	return original_buffer_size - buffer_size;

}
