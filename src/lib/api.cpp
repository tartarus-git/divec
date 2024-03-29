#include "compiler.h"

extern "C" {

	dive_program_t diveCreateProgram(const char *source_code) noexcept {
		return diveCreateProgram_inner(source_code);
	}

	divec_error_t diveCompileProgram(dive_program_t program) noexcept {
		return diveCompileProgram_inner(program);
	}

	divec_error_t diveReleaseProgram(dive_program_t program) noexcept {
		return diveReleaseProgram_inner(program);
	}

}
