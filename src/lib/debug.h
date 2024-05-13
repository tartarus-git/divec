#pragma once

#include "dive_ast.h"

namespace debug {

	void print_ast_indents(size_t indents) noexcept;

	void print_dive_ast_program(const dive_ast_program_t &program) noexcept;

	void print_dive_ast_function(const dive_ast_function_t &function, size_t indents) noexcept;

}
