#include "build_log.h"

#include <cstdlib>

dive_build_log_entry_t_inner::dive_build_log_entry_t_inner(dive_build_log_entry_type_t entry_type) noexcept
: entry_type(entry_type)
{ }

divec_error_t dive_build_log_entry_t_inner::base_free_children() noexcept {
	if (next != nullptr) {
		divec_error_t err = next->free_children();
		if (err != divec_error_t::SUCCESS) { return err; }
		std::free(next);
	}
	return divec_error_t::SUCCESS;
}

// USER-FACING FUNCTIONS

dive_build_log_t diveCreateBuildLog_inner(divec_error_t &err) noexcept {

	err = divec_error_t::SUCCESS;

	dive_build_log_t result = (dive_build_log_t)std::malloc(sizeof(dive_build_log_t_inner));
	if (result == nullptr) {
		err = divec_error_t::OUT_OF_MEMORY;
		return nullptr;
	}

	*result = {
		.entries = nullptr,
		.last_entry = nullptr
	};

	return result;

}

dive_build_log_entry_t diveGetNextBuildLogEntry_inner(dive_build_log_entry_t entry, divec_error_t &err) noexcept {
	err = divec_error_t::SUCCESS;
	return entry->next;
}

dive_build_log_entry_type_t diveGetBuildLogEntryType_inner(dive_build_log_entry_t entry, divec_error_t &err) noexcept {
	err = divec_error_t::SUCCESS;
	return entry->entry_type;
}

void* diveGetBuildLogEntryContents_inner(dive_build_log_entry_t entry, divec_error_t &err) noexcept {
	err = divec_error_t::SUCCESS;
	return entry->get_user_accessible_data_ptr();
}

size_t diveGetBuildLogEntryStringSize_inner(dive_build_log_entry_t entry, divec_error_t &err) noexcept {
	return entry->get_string_size(err);
}

size_t diveGetBuildLogEntryString_inner(dive_build_log_entry_t entry,
					char *buffer,
					size_t buffer_size,
					divec_error_t &err) noexcept
{
	return entry->get_string(buffer, buffer_size, err);
}

size_t diveGetBuildLogStringSize_inner(dive_build_log_t build_log, divec_error_t &err) noexcept {

	size_t sum = 0;

	for (dive_build_log_entry_t entry = build_log->entries; entry != nullptr; entry = entry->next) {
		sum += diveGetBuildLogEntryStringSize_inner(entry, err);
		if (err != divec_error_t::SUCCESS) { return -1; }
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

	for (dive_build_log_entry_t entry = build_log->entries; entry != nullptr; entry = entry->next) {

		size_t bytes_written = diveGetBuildLogEntryString_inner(entry, buffer, buffer_size, err);
		if (err != divec_error_t::SUCCESS) { return -1; }

		buffer += bytes_written;
		buffer_size -= bytes_written;

	}

	return original_buffer_size - buffer_size;

}

divec_error_t diveReleaseBuildLog_inner(dive_build_log_t build_log) noexcept {

	if (build_log->entries != nullptr) {
		divec_error_t err = build_log->entries->free_children();
		if (err != divec_error_t::SUCCESS) { return err; }
		std::free(build_log->entries);
	}

	std::free(build_log);

	return divec_error_t::SUCCESS;

}

// BACKEND FUNCTIONS

divec_error_t link_entry_to_build_log(dive_build_log_t build_log, dive_build_log_entry_t entry) noexcept {

	if (build_log->last_entry == nullptr) {
		if (build_log->entries != nullptr) { return divec_error_t::BUG; }

		build_log->entries = build_log->last_entry = entry;
		return divec_error_t::SUCCESS;
	}

	if (build_log->last_entry->next != nullptr) { return divec_error_t::BUG; }

	build_log->last_entry->next = entry;
	build_log->last_entry = build_log->last_entry->next;

	return divec_error_t::SUCCESS;

}
