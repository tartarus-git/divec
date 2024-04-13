#include "dive_token.h"

const char* get_dive_token_type_string(dive_token_type_t token_type) noexcept {

	switch (token_type) {
		case dive_token_type_t::INT32: return "<base_type>";
		case dive_token_type_t::ASTERISK: return "*";
		// TODO: etc...
		// TODO: Don't forget about the function in helpers.h that needs to be implemented.

		// TODO: Temp solution, replace later:
		default: return "[TEMP_TEXT]";
	}

}
