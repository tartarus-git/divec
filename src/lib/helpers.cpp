#include "helpers.h"

#include <format>

namespace helpers {

	divec_error_t convert_size_t_to_string(size_t input, std::string &output) noexcept {

		// TODO: Sadly, the C++ standard people were too stupid to include error handling here,
		// so program will crash if memory can't be allocated. Thanks for nothing.
		// Replace std::format with your own thing, since itoa isn't standard.
		// Or use one of the printf functions.
		output += std::format("{}", input);

		return divec_error_t::SUCCESS;

	}

	std::tuple<size_t, size_t> get_line_column_from_text_position(const char *text, size_t position) noexcept {
		size_t line = 1;
		size_t column = 1;

		for (const char *iter = text; iter - text < position; iter++) {
			switch (*iter) {
			case '\n': line++; column = 1; break;
			case '\0': return { (size_t)-1, (size_t)-1 };
			default: column++; break;
			}
		}

		return { line, column };
	}

}
