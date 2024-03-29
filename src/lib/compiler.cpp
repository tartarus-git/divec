#include "compiler.h"

dive_program_t diveCreateProgram_inner(const char *source_code) noexcept {

	dive_program_t result = new dive_program_t_inner {
		.state = dive_program_state_t::SOURCE,
		.source_code = source_code,
		.tokens = nullptr
	};

	return result;

}
