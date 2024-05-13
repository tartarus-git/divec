#include "debug.h"

#include <iostream>

namespace debug {

	void print_ast_indents(size_t indents) noexcept {
		for (size_t i = 0; i < indents; i++) { std::cout << ' '; }
	}

	void print_dive_ast_program(const dive_ast_program_t &program) noexcept {
		std::cout << "program\n";

		for (size_t i = 0; i < program.functions_length; i++)
		{
			print_dive_ast_function(*(program.functions[i]), 1);
		}
	}

	void print_dive_ast_function(const dive_ast_function_t &function, size_t indents) noexcept {
		print_ast_indents(indents);
		std::cout << "function " << (int)function.return_type.base_type << ' ';
		std::cout.write(function.name.value, function.name.value_length);
		std::cout << '\n';
	}

}
