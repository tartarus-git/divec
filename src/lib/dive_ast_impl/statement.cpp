#include "dive_ast.h"

divec_error_t dive_ast_statement_t::init() noexcept {
	return divec_error_t::SUCCESS;
}

divec_error_t dive_ast_statement_t::free_children() noexcept {
	return divec_error_t::SUCCESS;
}
