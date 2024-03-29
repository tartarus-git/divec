#include "compiler.h"

#define LIB_EXPORT __attribute__((visibility("default")))

extern "C" {

	LIB_EXPORT dive_program_t diveCreateProgram(const char *source_code) noexcept {
		return diveCreateProgram_inner(source_code);
	}

	LIB_EXPORT divec_error_t diveCompileProgram(dive_program_t program) noexcept {
		return diveCompileProgram_inner(program);
	}

	LIB_EXPORT divec_error_t diveReleaseProgram(dive_program_t program) noexcept {
		return diveReleaseProgram_inner(program);
	}

}
