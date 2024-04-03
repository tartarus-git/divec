#pragma once

#include <cstddef>
#include <cstdint>

// The AST structs are user-facing, as in part of the library API!
// That's why they are simple structs.
// The member functions are there for internal use only,
// they don't cross the API boundary.

enum class dive_ast_base_type_t {
	INT32
};

struct dive_ast_type_t {
	dive_ast_base_type_t base_type;
	// TODO: const and possibly other qualifiers
};

struct dive_ast_identifier_t {
	const char *value;

	divec_error_t free_children() noexcept;
};

// overestimated underlying type to ensure future API compatibility
enum class dive_ast_statement_type_t : uint16_t {
	COMPOUND_STATEMENT,
	ASSIGNMENT,
	EXPRESSION
};

struct dive_ast_statement_t {
	dive_ast_statement_type_t type;
	// TODO: other stuff
};

struct dive_ast_function_t {
	dive_ast_type_t return_type;
	dive_ast_identifier_t name;

	dive_ast_type_t *argument_types;
	dive_ast_identifier_t *argument_names;
	size_t arguments_length;
	size_t arguments_capacity;

	dive_ast_statement_t *body;

	divec_error_t init() noexcept;

	divec_error_t push_argument(dive_ast_type_t type, dive_ast_identifier_t identifier) noexcept;

	divec_error_t free_children() noexcept;
};

struct dive_ast_program_t {
	dive_ast_function_t *functions;
	size_t functions_length;
	size_t functions_capacity;

	divec_error_t init() noexcept;

	divec_error_t push_function(dive_ast_function_t *function) noexcept;

	divec_error_t free_children() noexcept;
};
