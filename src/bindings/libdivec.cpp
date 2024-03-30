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
	return dlopen(filename, RTLD_LAZY);
#elif defined(LIBDIVEC_PLATFORM_WINDOWS)
	return LoadLibraryA(file_name);
#endif
}

static bool unload_lib(shared_object_handle_t handle) noexcept {
#if defined(LIBDIVEC_PLATFORM_LINUX)
	return dlclose(handle);
#elif defined(LIBDIVEC_PLATFORM_WINDOWS)
	return FreeLibrary(handle);
#endif
}

static bound_function_handle_t bind_function(shared_object_handle_t lib_handle, const char *function_name) noexcept {
#if defined(LIBDIVEC_PLATFORM_LINUX)
	return dlsym(lib_handle, function_name);
#elif defined(LIBDIVEC_PLATFORM_WINDOWS)
	return GetProcAddress(lib_handle, function_name);
#endif
}

static shared_object_handle_t dive_lib_handle = nullptr;

divec_error_t diveInit() {
	if (dive_lib_handle != nullptr) { return DIVEC_ALREADY_DONE; }

	dive_lib_handle = load_lib("libdivec.so");
	if (dive_lib_handle != nullptr) { return DIVEC_BIND_LIB_FAILED; }

	return DIVEC_SUCCESS;
}

divec_error_t diveFree() {
	if (dive_lib_handle == nullptr) { return DIVEC_ALREADY_DONE; }

	if (unload_lib(dive_lib_handle) == false) { return DIVEC_BIND_LIB_FAILED; }

	return DIVEC_SUCCESS;
}

#define GENERATE_BINDER(function_name) static bound_function_handle_t bind_##function_name() noexcept { \
static bound_function_handle_t handle = nullptr; \
if (handle != nullptr) { return handle; } \
handle = bind_function(dive_lib_handle, #function_name); \
return handle; \
}

GENERATE_BINDER(diveCreateProgram)
dive_program_t diveCreateProgram(const char *source_code, divec_error_t *err) {
	*err = DIVEC_SUCCESS;
	bound_function_handle_t handle = bind_diveCreateProgram();
	if (handle == nullptr) { *err = DIVEC_BIND_LIB_FAILED; return nullptr; }
	return ((dive_program_t (*)(const char*))handle)(source_code);
}

divec_error_t divePrebind() {
	if (bind_diveCreateProgram() == nullptr) { return DIVEC_BIND_LIB_FAILED; }
	return DIVEC_SUCCESS;
}
