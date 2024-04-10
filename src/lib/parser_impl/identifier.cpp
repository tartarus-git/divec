#include "parser.h"

dive_ast_identifier_t parser_t::parse_identifier(dive_build_log_t build_log, divec_error_t &err) noexcept {

	err = divec_error_t::SUCCESS;

	dive_ast_identifier_t result;

	dive_token_t token = get_next_token();

	if (token.type != dive_token_type_t::IDENTIFIER) {
		err = divec_error_t::BUILD_ERROR;
		// TODO: push to build log
		return { };
	}

	result.value = program->source_code + token.begin;
	result.value_length = token.end - token.begin;

	return result;

}
