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

}
