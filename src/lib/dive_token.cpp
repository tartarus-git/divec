#include "dive_token.h"

const char* get_dive_token_type_string(dive_token_type_t token_type) noexcept {

	switch (token_type) {
		case dive_token_type_t::INT32: return "<base_type>";
		case dive_token_type_t::ASTERISK: return "*";
		case dive_token_type_t::OPENING_PARENTHESIS: return "(";
		case dive_token_type_t::CLOSING_PARENTHESIS: return ")";
		case dive_token_type_t::COMMA: return ",";
		// TODO: etc...

		// TODO: Temp solution, replace later:
		default: return "[TEMP_TEXT]";
	}

}
