#include "build_log.h"

#include <cstdlib>
#include <cstring>
#include <string>

#include "helpers.h"

dive_build_error_unexpected_token_t::dive_build_error_unexpected_token_t(size_t source_code_line,
									 size_t source_code_column,
									 const char *token_text,
									 size_t token_text_length,
									 dive_token_type_t *expected_token_types,
									 size_t expected_token_types_length,
									 divec_error_t &err) noexcept
: dive_build_log_entry_t_inner(dive_build_log_entry_type_t::ERROR_UNEXPECTED_TOKEN)
{
	err = divec_error_t::SUCCESS;

	user_accessible.source_code_line = source_code_line;
	user_accessible.source_code_column = source_code_column;

	user_accessible.token_text = (const char*)std::malloc((token_text_length + 1) * sizeof(char));
	if (user_accessible.token_text == nullptr) { err = divec_error_t::OUT_OF_MEMORY; return; }

	std::memcpy((char*)user_accessible.token_text, token_text, token_text_length * sizeof(char));
	((char*)(user_accessible.token_text))[token_text_length] = '\0';

	user_accessible.expected_token_types = expected_token_types;
	user_accessible.expected_token_types_length = expected_token_types_length;
}

divec_error_t dive_build_error_unexpected_token_t::gen_string_representation() noexcept {

	if (string_representation != nullptr) { return divec_error_t::SUCCESS; }

	std::string result;

	divec_error_t err = helpers::convert_size_t_to_string(user_accessible.source_code_line, result);
	if (err != divec_error_t::SUCCESS) { return err; }

	result += ':';

	err = helpers::convert_size_t_to_string(user_accessible.source_code_column, result);
	if (err != divec_error_t::SUCCESS) { return err; }

	result += ": error: unexpected token '";
	result += user_accessible.token_text;
	result += "', expected ";

	for (size_t i = 0; ; ) {
		result += '\'';
		result += get_dive_token_type_string(user_accessible.expected_token_types[i]);
		result += '\'';
		i++;
		if (i >= user_accessible.expected_token_types_length) { break; }
		result += " or ";
	}

	result += '\n';

	string_representation_size = result.length() * sizeof(char);

	string_representation = (const char*)std::malloc(string_representation_size);
	if (string_representation == nullptr) { return divec_error_t::OUT_OF_MEMORY; }

	std::memcpy((char*)(string_representation), result.c_str(), string_representation_size * sizeof(char));

	return divec_error_t::SUCCESS;

}

void* dive_build_error_unexpected_token_t::get_user_accessible_data_ptr() noexcept {
	return &user_accessible;
}

divec_error_t dive_build_error_unexpected_token_t::invalidate_string() noexcept {

	std::free((char*)string_representation);
	string_representation = nullptr;
	string_representation_size = 0;

	return divec_error_t::SUCCESS;

}

size_t dive_build_error_unexpected_token_t::get_string_size(divec_error_t &err) noexcept {

	err = divec_error_t::SUCCESS;

	err = gen_string_representation();
	if (err != divec_error_t::SUCCESS) { return -1; }

	return string_representation_size;

}

size_t dive_build_error_unexpected_token_t::get_string(char *buffer,
						       size_t buffer_size,
						       divec_error_t &err) noexcept
{

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

divec_error_t dive_build_error_unexpected_token_t::free_children() noexcept {

	std::free((char*)user_accessible.token_text);
	std::free((dive_token_type_t*)user_accessible.expected_token_types);
	std::free((char*)string_representation);

	divec_error_t err = base_free_children();
	if (err != divec_error_t::SUCCESS) { return divec_error_t::CATASTROPHIC_FAILURE; }

	return divec_error_t::SUCCESS;

}
