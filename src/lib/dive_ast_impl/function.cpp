#include "dive_ast.h"

#include "helpers.h"

// TODO: remove this include later
#include <iostream>

divec_error_t dive_ast_function_t::init() noexcept {

	return_type = { dive_ast_base_type_t::UNINITIALIZED };
	name = { nullptr, 0 };

	argument_types = nullptr;
	argument_names = nullptr;
	arguments_length = 0;
	arguments_capacity = 0;

	body = nullptr;

	return divec_error_t::SUCCESS;

}

divec_error_t dive_ast_function_t::push_argument(dive_ast_type_t type, dive_ast_identifier_t name) noexcept {

	size_t old_arguments_capacity = arguments_capacity;

	divec_error_t err = helpers::push_to_c_dynamic_array(argument_types,
							     arguments_length,
							     arguments_capacity,
							     type);
	if (err != divec_error_t::SUCCESS) { return err; }

	std::cout << "function push_argument got past pushing to the type array\n";

	// TODO: Why did you do this function the way you did?
	// Why does it cause segmentation fault?

	err = helpers::resize_c_dynamic_array(argument_names, old_arguments_capacity, arguments_capacity);
	if (err != divec_error_t::SUCCESS) { return err; }
	argument_names[arguments_length - 1] = name;

	std::cout << "function push_argument got past pushing to the name array\n";

	return divec_error_t::SUCCESS;

}

divec_error_t dive_ast_function_t::free_children() noexcept {

	divec_error_t err = body->free_children();
	if (err != divec_error_t::SUCCESS) { return err; }

	err = helpers::free_c_dynamic_array(argument_types, arguments_capacity);
	if (err != divec_error_t::SUCCESS) { return divec_error_t::CATASTROPHIC_FAILURE; }

	helpers::free_c_dynamic_array(argument_names, arguments_capacity);
	if (err != divec_error_t::SUCCESS) { return divec_error_t::CATASTROPHIC_FAILURE; }

	return divec_error_t::SUCCESS;

}
