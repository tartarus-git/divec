#include "parser.h"

const dive_token_t& parser_t::get_next_token() noexcept {

	// TODO: You should keep the EOF token in the token list, saves you from storing the length of the list
	// and it makes parsing more elegant as well.
	return *(tokens_head++);
}

const dive_token_t& parser_t::peek_token(size_t offset) noexcept {
	// The reason we don't write this more "efficiently" is because calculating tokens_head + offset
	// is UB if the resulting pointer is out-of-bounds of the array, even without dereferencing it.
	// Comparing the resulting value with tokens_end isn't UB, but it's implementation defined, so
	// that's not useful either.
	// That's why we do it the way we do it.
	// Compiler might be able to optimize a little bit, but it's easier said than done,
	// even on x86, which has a flat uniform memory model. The reason is that pointer overflow
	// is still an issue, no matter if it's handled by wrapping around or by triggering a trap.
	if (offset >= tokens_end - tokens_head) {
		return *tokens_end;
	}
	// TODO: You should probably scratch the above and just trust that the input is ok.
	// More efficient that way, faster.
	return *(tokens_head + offset);
}

parser_t::parser_t(dive_program_t program, divec_error_t &err) noexcept {
	err = divec_error_t::SUCCESS;

	switch (program->state) {
	case dive_program_state_t::SOURCE:
		err = divec_error_t::OBJECT_NOT_READY;
		return;
	case dive_program_state_t::LEXED:
		err = divec_error_t::OBJECT_NOT_READY;
		return;

	case dive_program_state_t::PREPROCESSED: break;

	default:
		err = divec_error_t::ALREADY_DONE;
		return;
	}

	this->program = program;
	tokens_head = program->tokens;
	tokens_end = program->tokens + program->tokens_length;
}

divec_error_t diveParseProgram_inner(dive_program_t program, dive_build_log_t build_log) noexcept {
	return divec_error_t::OUT_OF_MEMORY;
	// TODO: implement this function.
}
