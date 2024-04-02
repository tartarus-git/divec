#include "parser.h"

#include <cstdlib>

dive_ast_program_t* parser_t::parse_program(build_log_t *build_log, divec_error_t &err) noexcept {

	err = divec_error_t::SUCCESS;

	dive_ast_program_t *result = (dive_ast_program_t*)std::malloc(sizeof(dive_ast_program_t));
	if (result == nullptr) {
		err = divec_error_t::OUT_OF_MEMORY;
		return nullptr;
	}

	while (true) {
		dive_ast_function_t *function_entry = parse_function(build_log, err);
		if (err != divec_error_t::SUCCESS) { return nullptr; }

		result->push_function(function_entry);

		if (peek_token(1).type == dive_token_type_t::EOF_TOKEN) { break; }
	}

	return divec_error_t::SUCCESS;

}
