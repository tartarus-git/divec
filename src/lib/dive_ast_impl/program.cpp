#include "dive_ast.h"

#include <cstdlib>

#include "helpers.h"

divec_error_t dive_ast_program_t::init() noexcept {

	// TODO: You should probably set the array to nullptr, because the ast is user-facing
	// and the user will expect you to do that.
	// Same for function.cpp
	functions_length = 0;
	functions_capacity = 0;

	return divec_error_t::SUCCESS;

}

divec_error_t dive_ast_program_t::push_function(dive_ast_function_t *function) noexcept {

	divec_error_t err = helpers::push_to_c_dynamic_array(functions,
							     functions_length,
							     functions_capacity,
							     function);
	if (err != divec_error_t::SUCCESS) { return err; }

	return divec_error_t::SUCCESS;

}

divec_error_t dive_ast_program_t::free_children() noexcept {

	if (functions_length >= 1) {
		divec_error_t err = functions[0]->free_children();
		if (err != divec_error_t::SUCCESS) { return err; }
		std::free(functions[0]);

		for (size_t i = 1; i < functions_length; i++) {

			divec_error_t err = functions[i]->free_children();
			if (err != divec_error_t::SUCCESS) {
				return divec_error_t::CATASTROPHIC_FAILURE;
			}
			std::free(functions[i]);

		}
	}

	// even with length == 0 there can still be stuff that needs to be freed
	divec_error_t err = helpers::free_c_dynamic_array(functions, functions_capacity);
	if (err != divec_error_t::SUCCESS) { return divec_error_t::CATASTROPHIC_FAILURE; }

	return divec_error_t::SUCCESS;

}
