#include "dfa_gen.h"

std::vector<size_t> follow_ghost_rows(const nfa_table_t &nfa_table, const std::vector<size_t> &state_superposition) noexcept {
	std::vector<size_t> result;

	for (size_t i = 0; i < state_superposition.size(); i++) {
		const size_t state = state_superposition[i];
		const nfa_row_t &nfa_row = nfa_table.rows[state];

		if (is_ghost_row(nfa_row)) {
			// TODO: Make sure erase does what you think it does.
			for (const auto &child : nfa_row.children) {
				result.push_back(child);
			}
			continue;
		}

		result.push_back(state);
	}

	return result;
}

void shape_character(const dfa_table_t &dfa_table,
		     const nfa_table_t &nfa_table,
		     const std::vector<size_t> state_superposition,
		     uint8_t character) noexcept {
	// TODO: Recursive function to explore the graph and combine things.
}

dfa_table_t gen_dfa(const nfa_table_t &nfa_table) noexcept {

	dfa_table_t result;

	std::vector<size_t> state_superposition;
	state_superposition.push_back(0);

	while (true) {
		state_superpositions = follow_ghost_rows(nfa_table, state_superpositions);

		for (size_t i = 0; i < state_superposition.size(); i++) {
			const size_t state = state_superposition[i];
			const nfa_row_t &nfa_row = nfa_table.rows[state];

			for (const auto &nfa_element : nfa_row) {
				if (nfa_element.next != 0) {
				}
			}
		}
	}

	return result;
}
