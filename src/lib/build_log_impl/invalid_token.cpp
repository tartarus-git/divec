#include "build_log.h"

#include <cstdlib>
#include <cstring>
#include <string>

#include "helpers.h"

dive_build_error_invalid_token_t::dive_build_error_invalid_token_t(size_t source_code_line,
								   size_t source_code_column,
								   const char *invalid_token_text,
								   size_t invalid_token_text_size,
								   divec_error_t &err) noexcept
: dive_build_log_entry_t_inner(dive_build_log_entry_type_t::ERROR_INVALID_TOKEN)
{
	err = divec_error_t::SUCCESS;

	user_accessible.source_code_line = source_code_line;
	user_accessible.source_code_column = source_code_column;

	user_accessible.invalid_token_text = (const char*)std::malloc(invalid_token_text_size + 1 * sizeof(char));
	if (user_accessible.invalid_token_text == nullptr) { err = divec_error_t::OUT_OF_MEMORY; return; }

	std::memcpy((char*)(user_accessible.invalid_token_text), invalid_token_text, invalid_token_text_size);
	((char*)(user_accessible.invalid_token_text))[invalid_token_text_size / sizeof(char)] = '\0';
}

divec_error_t dive_build_error_invalid_token_t::gen_string_representation() noexcept {

	if (string_representation != nullptr) { return divec_error_t::SUCCESS; }

	// TODO: Replace with your own orphanable string and other improvements. Same for other build log impl files. Everywhere really in the whole codebase.
	std::string result;

	divec_error_t err = helpers::convert_size_t_to_string(user_accessible.source_code_line, result);
	if (err != divec_error_t::SUCCESS) { return err; }

	result += ':';

	err = helpers::convert_size_t_to_string(user_accessible.source_code_column, result);
	if (err != divec_error_t::SUCCESS) { return err; }

	result += ": error: invalid token '";
	result += user_accessible.invalid_token_text;
	result += "'\n";

	string_representation_size = result.length() * sizeof(char);

	string_representation = (const char*)std::malloc(string_representation_size);
	if (string_representation == nullptr) { return divec_error_t::OUT_OF_MEMORY; }

	std::memcpy((char*)string_representation, result.c_str(), string_representation_size * sizeof(char));

	return divec_error_t::SUCCESS;

}

void* dive_build_error_invalid_token_t::get_user_accessible_data_ptr() noexcept {
	return &user_accessible;
}

size_t dive_build_error_invalid_token_t::get_string_size(divec_error_t &err) noexcept {

	err = divec_error_t::SUCCESS;

	err = gen_string_representation();
	if (err != divec_error_t::SUCCESS) { return -1; }

	return string_representation_size;

}

size_t dive_build_error_invalid_token_t::get_string(char *buffer, size_t buffer_size, divec_error_t &err) noexcept {

	err = divec_error_t::SUCCESS;

	err = gen_string_representation();
	if (err != divec_error_t::SUCCESS) { return -1; }

	if (buffer_size < string_representation_size) {
		err = divec_error_t::BUFFER_TOO_SMALL;
		return -1;
	}

	std::memcpy(buffer, string_representation, string_representation_size);

	return string_representation_size;

}

divec_error_t dive_build_error_invalid_token_t::free_children() noexcept {

	std::free((char*)user_accessible.invalid_token_text);
	std::free((char*)string_representation);

	divec_error_t err = base_free_children();
	if (err != divec_error_t::SUCCESS) { return divec_error_t::CATASTROPHIC_FAILURE; }

	return divec_error_t::SUCCESS;

}
