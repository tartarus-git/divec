#include "parser.h"

dive_ast_type_t parser_t::parse_type(dive_build_log_t build_log, divec_error_t &err) noexcept {

	err = divec_error_t::SUCCESS;

	dive_ast_type_t result;

	switch (peek_token(1).type) {
		// TODO: Check for const modifiers or whatever.
		default: break;
	}

	switch (get_next_token().type) {
	case dive_token_type_t::INT32: result.base_type = dive_ast_base_type_t::INT32; break;
	case dive_token_type_t::INT64: result.base_type = dive_ast_base_type_t::INT64; break;
	default:
		err = divec_error_t::BUILD_ERROR;
		// TODO: push to build log.
		return { };
	}

	return { };

}
