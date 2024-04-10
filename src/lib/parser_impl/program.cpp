#include "parser.h"

#include <cstdlib>

// TODO: Remove this later.
#include <iostream>

dive_ast_program_t* parser_t::parse_program(dive_build_log_t build_log, divec_error_t &err) noexcept {

	err = divec_error_t::SUCCESS;

	dive_ast_program_t *result = (dive_ast_program_t*)std::malloc(sizeof(dive_ast_program_t));
	if (result == nullptr) {
		err = divec_error_t::OUT_OF_MEMORY;
		return nullptr;
	}

	result->init();

	std::cout << "Got to just before parse function!\n";

	while (true) {
		dive_ast_function_t *function_entry = parse_function(build_log, err);
		if (err != divec_error_t::SUCCESS) {
			std::free(result);
			return nullptr;
		}

		err = result->push_function(function_entry);
		if (err != divec_error_t::SUCCESS) {

			divec_error_t sub_err = function_entry->free_children();
			if (sub_err != divec_error_t::SUCCESS) {
				err = divec_error_t::CATASTROPHIC_FAILURE;
				// FALLTHROUGH
			}

			std::free(function_entry);
			std::free(result);
			return nullptr;

		}

		std::cout << "Finished parsing function!\n";

		if (peek_token(0).type == dive_token_type_t::EOF_TOKEN) { break; }
	}

	return result;

}
