#include "unexpected_token.h"

dive_build_error_invalid_token_t::dive_build_error_invalid_token_t() noexcept
: string_representation(nullptr)
{ }

divec_error_t dive_build_error_invalid_token_t::gen_string_representation(dive_build_log_t build_log) noexcept {

	return divec_error_t::OUT_OF_MEMORY;
	// TODO: implement

	// TODO: use the size_t to string function in helpers.h and the token type to string function in
	// dive_token.h. Create a nice message. Think about how the messages are going to be structured.
	// You really gotta take care with this. Make the system great from the start. Good error messages are crucial.

}

size_t dive_build_error_invalid_token_t::get_string_size(dive_build_log_t build_log) noexcept {
	return -1;
	// TODO: implement
}

size_t dive_build_error_invalid_token_t::get_string(dive_build_log_t build_log,
						    char *buffer,
						    size_t buffer_size,
						    divec_error_t &err) noexcept
{
	return divec_error_t::OUT_OF_MEMORY;
	// TODO: implement
}
