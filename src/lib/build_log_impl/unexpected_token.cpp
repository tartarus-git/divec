#include "build_log.h"

#include <cstdlib>
#include <cstring>
#include <string>

#include "helpers.h"

dive_build_error_unexpected_token_t::dive_build_error_unexpected_token_t() noexcept
: dive_build_log_entry_t_inner(dive_build_log_entry_type_t::ERROR_UNEXPECTED_TOKEN)
{
	user_accessible.string_representation = nullptr;
}

divec_error_t dive_build_error_unexpected_token_t::gen_string_representation() noexcept {

	if (user_accessible.string_representation != nullptr) { return divec_error_t::SUCCESS; }

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

	user_accessible.string_representation_size = result.length() * sizeof(char);

	user_accessible.string_representation = (const char*)std::malloc(user_accessible.string_representation_size);
	if (user_accessible.string_representation == nullptr) { return divec_error_t::OUT_OF_MEMORY; }

	std::memcpy((char*)(user_accessible.string_representation), result.c_str(), user_accessible.string_representation_size * sizeof(char));

	return divec_error_t::SUCCESS;

}

void* dive_build_error_unexpected_token_t::get_user_accessible_data_ptr() noexcept {
	return &user_accessible;
}

size_t dive_build_error_unexpected_token_t::get_string_size(divec_error_t &err) noexcept {

	err = divec_error_t::SUCCESS;

	err = gen_string_representation();
	if (err != divec_error_t::SUCCESS) { return -1; }

	return user_accessible.string_representation_size;

}

size_t dive_build_error_unexpected_token_t::get_string(char *buffer,
						       size_t buffer_size,
						       divec_error_t &err) noexcept
{

	err = divec_error_t::SUCCESS;

	err = gen_string_representation();
	if (err != divec_error_t::SUCCESS) { return -1; }

	if (buffer_size < user_accessible.string_representation_size) {
		err = divec_error_t::BUFFER_TOO_SMALL;
		return -1;
	}

	std::memcpy(buffer, user_accessible.string_representation, user_accessible.string_representation_size);

	return user_accessible.string_representation_size;

}

divec_error_t dive_build_error_unexpected_token_t::free_children() noexcept {

	std::free((char*)user_accessible.token_text);
	std::free((dive_token_type_t*)user_accessible.expected_token_types);
	std::free((char*)user_accessible.string_representation);

	divec_error_t err = base_free_children();
	if (err != divec_error_t::SUCCESS) { return divec_error_t::CATASTROPHIC_FAILURE; }

	return divec_error_t::SUCCESS;

}
