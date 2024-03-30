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

	enum divec_error_t : uint8_t {
		DIVEC_SUCCESS,
		DIVEC_OUT_OF_MEMORY,
		DIVEC_ALREADY_DONE,
		DIVEC_BUILD_ERROR,
		DIVEC_BIND_LIB_FAILED
	};

	typename struct dive_build_log_t_inner* dive_build_log_t;
	typename struct dive_program_t_inner* dive_program_t;

	divec_error_t diveInit();

	dive_program_t diveCreateProgram(const char *source_code);

	divec_error_t diveCompileProgram(dive_program_t program, dive_build_log_t build_log);

#ifdef __cplusplus
}
#endif

#endif
