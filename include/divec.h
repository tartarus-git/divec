#ifndef LIBDIVEC_BINDINGS_HEADER_INCLUDED
#define LIBDIVEC_BINDINGS_HEADER_INCLUDED

#include <stdint.h>	// We have to use C-style headers here because this has to work in C and in C++.

#ifdef __cplusplus
extern "C" {
#endif

	// We overestimate width of this type, see note in compiler_error.h
	enum divec_error_t : uint32_t {
		DIVEC_SUCCESS,
		DIVEC_OUT_OF_MEMORY,
		DIVEC_ALREADY_DONE,
		DIVEC_OBJECT_NOT_READY,
		DIVEC_BUFFER_TOO_SMALL,
		DIVEC_BUILD_ERROR,
		DIVEC_GENERAL_FAILURE,
		DIVEC_CATASTROPHIC_FAILURE,	// Used for when the best option is for program to kill itself.
						// Used (for example) when the program state is corrupted.
		BUG			// Used when the library is able to recognize that it contains a bug.
					// Only thing handler can reasonably do here is terminate program, after which you can post an issue on the library GitHub.
	};

	typedef struct dive_build_log_t_inner* dive_build_log_t;
	typedef struct dive_program_t_inner* dive_program_t;

	dive_program_t diveCreateProgram(const char *source_code);
	divec_error_t diveCompileProgram(dive_program_t program, dive_build_log_t build_log);
	divec_error_t diveReleaseProgram(dive_program_t program);

	dive_build_log_t diveCreateBuildLog(divec_error_t *err);
	size_t diveGetBuildLogStringSize(dive_build_log_t build_log, divec_error_t *err);
	size_t diveGetBuildLogString(dive_build_log_t build_log, char *buffer, size_t buffer_size, divec_error_t *err);

#ifdef __cplusplus
}
#endif

#endif
