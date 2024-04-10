#include "program.h"
#include "lexer.h"
#include "preprocessor.h"
#include "parser.h"

#define LIB_EXPORT __attribute__((visibility("default")))

extern "C" {

	LIB_EXPORT dive_program_t diveCreateProgram(const char *source_code) noexcept {
		return diveCreateProgram_inner(source_code);
	}

	LIB_EXPORT divec_error_t diveLexProgram(dive_program_t program, dive_build_log_t build_log) noexcept {
		return diveLexProgram_inner(program, build_log);
	}

	LIB_EXPORT divec_error_t divePreprocessProgram(dive_program_t program, dive_build_log_t build_log) noexcept {
		return divePreprocessProgram_inner(program, build_log);
	}

	LIB_EXPORT divec_error_t diveParseProgram(dive_program_t program, dive_build_log_t build_log) noexcept {
		return diveParseProgram_inner(program, build_log);
	}

	LIB_EXPORT divec_error_t diveCompileProgram(dive_program_t program, dive_build_log_t build_log) noexcept {
		return diveCompileProgram_inner(program, build_log);
	}

	LIB_EXPORT divec_error_t diveReleaseProgram(dive_program_t program) noexcept {
		return diveReleaseProgram_inner(program);
	}

}
