#include "compiler_error.h"
#include "lexer.h"
#include "preprocessor.h"
#include "parser.h"
#include "compiler.h"
#include "build_log.h"

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

	LIB_EXPORT dive_build_log_t diveCreateBuildLog(divec_error_t *err) noexcept {
		// forwarding err through a reference
		return diveCreateBuildLog_inner(*err);
	}

	LIB_EXPORT size_t diveGetBuildLogEntryStringSize(dive_build_log_entry_t entry, divec_error_t *err) noexcept {
		// forwarding err through a reference
		return diveGetBuildLogEntryStringSize_inner(entry, *err);
	}

	LIB_EXPORT size_t diveGetBuildLogEntryString(dive_build_log_entry_t entry,
						     char *buffer,
						     size_t buffer_size,
						     divec_error_t *err) noexcept
	{
		// forwarding err through a reference
		return diveGetBuildLogEntryString_inner(entry, buffer, buffer_size, *err);
	}

	LIB_EXPORT size_t diveGetBuildLogStringSize(dive_build_log_t build_log, divec_error_t *err) noexcept {
		// forwarding err through a reference
		return diveGetBuildLogStringSize_inner(build_log, *err);
	}

	LIB_EXPORT size_t diveGetBuildLogString(dive_build_log_t build_log, char *buffer, size_t buffer_size, divec_error_t *err) noexcept {
		// forwarding err through a reference
		return diveGetBuildLogString_inner(build_log, buffer, buffer_size, *err);
	}

	LIB_EXPORT divec_error_t diveReleaseBuildLog(dive_build_log_t build_log) noexcept {
		return diveReleaseBuildLog_inner(build_log);
	}

}
