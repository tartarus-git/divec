#pragma once

#include "compiler_error.h"
#include "build_log.h"
#include "dive_token.h"
#include "dive_ast.h"

class parser_t {
	dive_program_t program;

	const dive_token_t *tokens_head;
	const dive_token_t *tokens_end;

	dive_token_t get_next_token() noexcept;
	dive_token_t peek_token(size_t offset) noexcept;

public:
	parser_t(dive_program_t program, divec_error_t &err) noexcept;

	dive_ast_program_t parse_program(dive_build_log_t build_log, divec_error_t &err) noexcept;
	dive_ast_function_t parse_function(dive_build_log_t build_log, divec_error_t &err) noexcept;
	//divec_error_t parse_if_statement(build_log_t *build_log) noexcept;
	//divec_error_t parse_while_loop(build_log_t *build_log) noexcept;
	//divec_error_t parse_for_loop(build_log_t *build_log) noexcept;
	dive_ast_statement_t parse_statement(dive_build_log_t build_log, divec_error_t &err) noexcept;
	//divec_error_t parse_expression(build_log_t *build_log) noexcept;
};
