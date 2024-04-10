#include "parser.h"

#include <cstdlib>

dive_ast_statement_t* parser_t::parse_statement(dive_build_log_t build_log, divec_error_t &err) noexcept {

	err = divec_error_t::SUCCESS;

	dive_ast_statement_t *result = (dive_ast_statement_t*)std::malloc(sizeof(dive_ast_statement_t));
	if (result == nullptr) {
		err = divec_error_t::OUT_OF_MEMORY;
		return nullptr;
	}

	result->init();

	result->type = dive_ast_statement_type_t::EXPRESSION;

	if (get_next_token().type != dive_token_type_t::OPENING_ANGEL_BRACKET) {
		err = divec_error_t::BUILD_ERROR;
		std::free(result);
		// TODO: Same todos as below.
		return nullptr;
	}

	if (get_next_token().type != dive_token_type_t::IDENTIFIER) {
		err = divec_error_t::BUILD_ERROR;
		// TODO: append to build log
		std::free(result);
		// TODO: consider free_children, and also put it in program.cpp as well if your gonan put it everywhere else.
		return nullptr;
	}

	if (get_next_token().type != dive_token_type_t::CLOSING_ANGEL_BRACKET) {
		err = divec_error_t::BUILD_ERROR;
		std::free(result);
		// TODO: Same todos as above.
		return nullptr;
	}

	return result;

}
