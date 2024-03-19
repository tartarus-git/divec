#pragma once

#include <vector>
#include <cstdint>

enum class regex_token_type_t : uint8_t {
	FILTER,
	ALTERNATION,
	SUBEXPRESSION_BEGIN,
	SUBEXPRESSION_END,
	KLEENE_STAR
};

struct regex_token_t {
	regex_token_type_t type;
	bool row_filter[256];
};

struct nfa_element_t {
	// NOTE: negative values for next are the negative indices of the tokens that
	// are being matched.
	size_t next;
};

struct nfa_row_t {
	bool ghost_row;
	nfa_element_t elements[256];
	std::vector<size_t> children;
};

struct nfa_table_t {
	std::vector<nfa_row_t> rows;
};
