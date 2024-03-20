#pragma once

#include <vector>

struct dfa_element_t {
	// NOTE: negatives mean indices of the matches that are now matched.
	size_t next;
};

struct dfa_row_t {
	dfa_element_t elements[256];
};

struct dfa_table_t {
	std::vector<dfa_row_t> rows;
};

dfa_table_t gen_dfa(const nfa_table_t &nfa_table) noexcept;
