#ifndef LIBDIVEC_BINDINGS_HEADER_INCLUDED
#define LIBDIVEC_BINDINGS_HEADER_INCLUDED

// This is done for projects that combine C and C++.
// These symbols need to refer to the same things in all translation units,
// regardless of language. This is accomplished by turning off name-mangling
// when header is compiled as C++.
// ALSO, we need to do this anyway because we have the implementations of these
// functions in a static library, which exports them with C linkage, for obvious reasons.
#ifdef __cplusplus
extern "C" {
#endif

	// TODO: Think about which calling convention is the best for this.

	// We DON'T explicitly set the underlying type, see note in compiler_error.h file.
	enum divec_error_t {
		DIVEC_SUCCESS,
		DIVEC_OUT_OF_MEMORY,
		DIVEC_ALREADY_DONE,
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

	divec_error_t diveInit();
	divec_error_t diveFree();

#ifdef __cplusplus
}
#endif

#endif
