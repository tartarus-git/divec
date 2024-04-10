#include "parser.h"

#include <cstdlib>

dive_ast_function_t* parser_t::parse_function(dive_build_log_t build_log, divec_error_t &err) noexcept {

	err = divec_error_t::SUCCESS;

	dive_ast_function_t *result = (dive_ast_function_t*)std::malloc(sizeof(dive_ast_function_t));
	if (result == nullptr) {
		err = divec_error_t::OUT_OF_MEMORY;
		return nullptr;
	}

	result->init();

	result->return_type = parse_type(build_log, err);
	if (err != divec_error_t::SUCCESS) {
		std::free(result);
		// TODO: consider calling free_children here.
		return nullptr;
	}

	result->name = parse_identifier(build_log, err);
	if (err != divec_error_t::SUCCESS) {
		std::free(result);
		return nullptr;
	}

	if (get_next_token().type != dive_token_type_t::OPENING_PARENTHESIS) {
		err = divec_error_t::BUILD_ERROR;
		// TODO: append it to build log.
		std::free(result);
		return nullptr;
	}

	if (peek_token(1).type != dive_token_type_t::CLOSING_PARENTHESIS) {

		while (true) {

			dive_ast_type_t argument_type = parse_type(build_log, err);
			if (err != divec_error_t::SUCCESS) {
				result->free_children();
				std::free(result);
				return nullptr;
			}

			dive_ast_identifier_t argument_name = parse_identifier(build_log, err);
			if (err != divec_error_t::SUCCESS) {
				result->free_children();
				std::free(result);
				return nullptr;
			}

			err = result->push_argument(argument_type, argument_name);
			if (err != divec_error_t::SUCCESS) {
				result->free_children();
				std::free(result);
				return nullptr;
			}

			if (peek_token(1).type == dive_token_type_t::CLOSING_PARENTHESIS) { break; }

			if (get_next_token().type != dive_token_type_t::COMMA) {
				err = divec_error_t::BUILD_ERROR;
				// TODO: append it to build log
				result->free_children();
				std::free(result);
				return nullptr;
			}

		}

	}

	// take parenthesis from queue
	get_next_token();

	if (peek_token(1).type != dive_token_type_t::OPENING_ANGEL_BRACKET) {
		err = divec_error_t::BUILD_ERROR;
		// TODO: append it to build log
		result->free_children();
		std::free(result);
		return nullptr;
	}

	result->body = parse_statement(build_log, err);
	if (err != divec_error_t::SUCCESS) {
		result->free_children();
		std::free(result);
		return nullptr;
	}

	return result;

}
