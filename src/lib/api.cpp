#include "compiler_error.h"
#include "lexer.h"
#include "preprocessor.h"
#include "parser.h"
#include "compiler.h"

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

	LIB_EXPORT dive_build_log_t diveCreateBuildLog() noexcept {
		return diveCreateBuildLog_inner();
	}

	LIB_EXPORT size_t diveGetBuildLogEntryStringSize(dive_build_log_t build_log, void *entry) noexcept {
		return diveGetBuildLogEntryStringSize_inner(build_log, entry);
	}

	LIB_EXPORT size_t diveGetBuildLogEntryString(dive_build_log_t build_log,
						  void *entry,
						  char *buffer,
						  size_t buffer_size,
						  divec_error_t *err) noexcept
	{
		// forwarding err through a reference
		return diveBuildLogEntryString_inner(build_log, entry, buffer, buffer_size, *err);
	}

	LIB_EXPORT size_t diveGetBuildLogStringSize(dive_build_log_t build_log) noexcept {
		return diveGetBuildLogStringSize_inner(build_log);
	}

	LIB_EXPORT size_t diveGetBuildLogString(dive_build_log_t build_log, char *buffer, size_t buffer_size, divec_error_t *err) noexcept {
		// forwarding err through a reference
		return diveGetBuildLogString_inner(build_log, buffer, buffer_size, *err);
	}

	LIB_EXPORT divec_error_t diveReleaseBuildLog() noexcept {
		return diveReleaseBuildLog_inner();
	}

}
