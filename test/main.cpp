#include <iostream>
#include <cstdlib>

#include "divec.h"

constexpr char test_source_code[] = R"(
int32_t test = 1;
)";

int main() {
	std::cout << "creating program...\n";
	dive_program_t dive_program = diveCreateProgram(test_source_code);
	if (dive_program == nullptr) {
		std::cerr << "libdivec test: error: failed to create dive program\n";
		std::exit(EXIT_FAILURE);
	}

	std::cout << "compiling program...\n";
	divec_error_t err = diveCompileProgram(dive_program, nullptr);
	switch (err) {
	case DIVEC_SUCCESS: break;
	case DIVEC_ALREADY_DONE:
		std::cerr << "libdivec test: error: already lexed\n";
		std::exit(EXIT_FAILURE);
	case DIVEC_BUILD_ERROR:
		std::cerr << "libdivec test: error: build error\n";
		std::exit(EXIT_FAILURE);
	case DIVEC_OUT_OF_MEMORY:
		std::cerr << "libdivec test: error: out of memory\n";
		std::exit(EXIT_FAILURE);
	default:
		std::cerr << "libdivec test: error: unknown error\n";
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
