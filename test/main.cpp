#include <iostream>
#include <cstdlib>

#include "divec.h"

constexpr char test_source_code[] = R"(
int32_t func1(){int32_t}
)";

void outputBuildLog(dive_build_log_t build_log) noexcept {
	divec_error_t err = DIVEC_SUCCESS;
	size_t string_size = diveGetBuildLogStringSize(build_log, &err);
	if (err != DIVEC_SUCCESS) {
		std::cerr << "libdivec test: error: in outputBuildLog in diveGetBuildLogStringSize something happened\n";
		std::exit(EXIT_FAILURE);
	}

	char *string_buffer = new char[string_size / sizeof(char) + 1];

	diveGetBuildLogString(build_log, string_buffer, string_size, &err);
	if (err != DIVEC_SUCCESS) {
		std::cerr << "libdivec test: error: in outputBuildLog in diveGetBuildLogString something happened\n";
		std::exit(EXIT_FAILURE);
	}

	string_buffer[string_size / sizeof(char)] = '\0';

	std::cout << string_buffer << '\n';

	delete[] string_buffer;
}

int main() {
	std::cout << "creating program...\n";
	dive_program_t dive_program = diveCreateProgram(test_source_code);
	if (dive_program == nullptr) {
		std::cerr << "libdivec test: error: failed to create dive program\n";
		std::exit(EXIT_FAILURE);
	}

	divec_error_t err;
	dive_build_log_t build_log = diveCreateBuildLog(&err);
	if (err != DIVEC_SUCCESS) {
		std::cerr << "libdivec test: error: diveCreateBuildLog failed\n";
		std::exit(EXIT_FAILURE);
	}

	std::cout << "compiling program...\n";
	err = diveCompileProgram(dive_program, build_log);
	switch (err) {
	case DIVEC_SUCCESS: break;
	case DIVEC_ALREADY_DONE:
		std::cerr << "libdivec test: error: already lexed\n";
		std::exit(EXIT_FAILURE);
	case DIVEC_BUILD_ERROR:
		std::cerr << "libdivec test: error: build error\n";

		outputBuildLog(build_log);

		std::exit(EXIT_FAILURE);
	case DIVEC_OUT_OF_MEMORY:
		std::cerr << "libdivec test: error: out of memory\n";
		std::exit(EXIT_FAILURE);
	default:
		std::cerr << "libdivec test: error: unknown error\n";
		std::cerr << "err number: " << err << '\n';
		std::exit(EXIT_FAILURE);
	}

	std::cout << "releasing program...\n";
	err = diveReleaseProgram(dive_program);
	switch (err) {
	case DIVEC_SUCCESS: break;
	default:
		std::cerr << "libdivec test: error: failed to release dive program\n";
		std::exit(EXIT_FAILURE);
	}
}
