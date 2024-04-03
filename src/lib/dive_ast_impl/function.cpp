#include "dive_ast.h"

#include "helpers.h"

divec_error_t dive_ast_function_t::init() noexcept {

	arguments_length = 0;
	arguments_capacity = 0;

	return divec_error_t::SUCCESS;

}

divec_error_t dive_ast_function_t::push_argument(dive_ast_type_t type, dive_ast_identifier_t name) noexcept {

	size_t old_arguments_length = arguments_length;

	divec_error_t err = helpers::push_to_c_dynamic_array(argument_types,
							     arguments_length,
							     arguments_capacity,
							     type);
	if (err != divec_error_t::SUCCESS) { return err; }

	err = helpers::resize_c_dynamic_array(argument_names, old_arguments_length, arguments_length, arguments_capacity);
	if (err != divec_error_t::SUCCESS) { return err; }
	argument_names[arguments_length - 1] = name;

	return divec_error_t::SUCCESS;

}

divec_error_t dive_ast_function_t::free_children() noexcept {

	divec_error_t err = body->free_children();
	if (err != divec_error_t::SUCCESS) { return err; }

	err = helpers::free_c_dynamic_array(argument_types, arguments_length);
	if (err != divec_error_t::SUCCESS) { return divec_error_t::CATASTROPHIC_FAILURE; }

	helpers::free_c_dynamic_array(argument_names, arguments_length);
	if (err != divec_error_t::SUCCESS) { return divec_error_t::CATASTROPHIC_FAILURE; }

	return divec_error_t::SUCCESS;

}
