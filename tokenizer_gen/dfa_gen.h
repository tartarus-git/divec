#pragma once

#include <vector>

#include "nfa_gen.h"

struct dfa_element_t {
	size_t next;
};

struct dfa_row_t {
	dfa_element_t elements[257];
	size_t token_id;
};

struct dfa_table_t {
	std::vector<dfa_row_t> rows;
};

dfa_table_t gen_dfa(const nfa_table_t &nfa_table) noexcept;
