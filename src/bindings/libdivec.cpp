#include "divec.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define LIBDIVEC_PLATFORM_WINDOWS
#else
#define LIBDIVEC_PLATFORM_LINUX
#endif

#if defined(LIBDIVEC_PLATFORM_LINUX)

#include <dlfcn.h>

using shared_object_handle_t = void*;
using bound_function_handle_t = void*;

#elif defined(LIBDIVEC_PLATFORM_WINDOWS)

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using shared_object_handle_t = HMODULE;
using bound_function_handle_t = FARPROC;

#endif

static shared_object_handle_t load_lib(const char *file_name) noexcept {
#if defined(LIBDIVEC_PLATFORM_LINUX)
	return dlopen(file_name, RTLD_LAZY);
#elif defined(LIBDIVEC_PLATFORM_WINDOWS)
	return LoadLibraryA(file_name);
#endif
}

static bound_function_handle_t bind_function(shared_object_handle_t lib_handle, const char *function_name) noexcept {
#if defined(LIBDIVEC_PLATFORM_LINUX)
	return dlsym(lib_handle, function_name);
#elif defined(LIBDIVEC_PLATFORM_WINDOWS)
	return GetProcAddress(lib_handle, function_name);
#endif
}

static bool unload_lib(shared_object_handle_t handle) noexcept {
#if defined(LIBDIVEC_PLATFORM_LINUX)
	return dlclose(handle);
#elif defined(LIBDIVEC_PLATFORM_WINDOWS)
	return FreeLibrary(handle);
#endif
}

static shared_object_handle_t dive_lib_handle = nullptr;

#ifdef __cplusplus
extern "C" {
#endif

	// TODO: Think about which calling convention is the best option here.

	static dive_program_t (*diveCreateProgram_ptr)(const char*);
	dive_program_t diveCreateProgram(const char *source_code) {
		return diveCreateProgram_ptr(source_code);
	}

	static divec_error_t (*diveCompileProgram_ptr)(dive_program_t program, dive_build_log_t build_log);
	divec_error_t diveCompileProgram(dive_program_t program, dive_build_log_t build_log) {
		return diveCompileProgram_ptr(program, build_log);
	}

	static divec_error_t (*diveReleaseProgram_ptr)(dive_program_t program);
	divec_error_t diveReleaseProgram(dive_program_t program) {
		return diveReleaseProgram_ptr(program);
	}

	static bool bind_all_functions() noexcept {
		diveCreateProgram_ptr = (decltype(diveCreateProgram_ptr))bind_function(dive_lib_handle, "diveCreateProgram");
		if (diveCreateProgram_ptr == nullptr) { return false; }

		diveCompileProgram_ptr = (decltype(diveCompileProgram_ptr))bind_function(dive_lib_handle,
											 "diveCompileProgram");
		if (diveCompileProgram_ptr == nullptr) { return false; }

		diveReleaseProgram_ptr = (decltype(diveReleaseProgram_ptr))bind_function(dive_lib_handle,
											 "diveReleaseProgram");
		if (diveReleaseProgram_ptr == nullptr) { return false; }

		return true;
	}

	divec_error_t diveInit() {
		if (dive_lib_handle != nullptr) { return DIVEC_ALREADY_DONE; }

		dive_lib_handle = load_lib("libdivec.so");
		if (dive_lib_handle != nullptr) { return DIVEC_GENERAL_FAILURE; }

		if (bind_all_functions() == false) {
			if (unload_lib(dive_lib_handle) == false) { return DIVEC_CATASTROPHIC_FAILURE; }
			return DIVEC_GENERAL_FAILURE;
		}

		return DIVEC_SUCCESS;
	}

	divec_error_t diveFree() {
		if (dive_lib_handle == nullptr) { return DIVEC_ALREADY_DONE; }

		// We don't return DIVEC_CATASTROPHIC_FAILURE here because unlike in diveInit(),
		// the state isn't corrupted if this fails.
		if (unload_lib(dive_lib_handle) == false) { return DIVEC_GENERAL_FAILURE; }

		return DIVEC_SUCCESS;
	}

#ifdef __cplusplus
}
#endif
