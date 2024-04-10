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
		OBJECT_NOT_READY,
		DIVEC_BUILD_ERROR,
		DIVEC_GENERAL_FAILURE,
		DIVEC_CATASTROPHIC_FAILURE	// Used for when the best option is for program to kill itself.
						// Used (for example) when the program state is corrupted.
	};

	typedef struct dive_build_log_t_inner* dive_build_log_t;
	typedef struct dive_program_t_inner* dive_program_t;

	dive_program_t diveCreateProgram(const char *source_code);
	divec_error_t diveCompileProgram(dive_program_t program, dive_build_log_t build_log);
	divec_error_t diveReleaseProgram(dive_program_t program);

#ifdef __cplusplus
}
#endif

#endif
