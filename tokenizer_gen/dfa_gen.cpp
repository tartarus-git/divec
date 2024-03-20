#include "dfa_gen.h"

#include "nfa_gen.h"

#include <vector>
#include <cstdint>
#include <iostream>

bool superpositions_equal(const std::vector<size_t> &a, const std::vector<size_t> &b) noexcept {

	// sets have to have the same size
	if (a.size() != b.size()) { return false; }

	// the one set has to be contained within the other
	for (const size_t &state : a) {
		bool hit = false;
		for (const size_t &other_state : b) {
			if (state == other_state) {
				hit = true;
				break;
			}
		}
		if (hit == false) { return false; }
	}

	// the other one has to be contained within the former
	for (const size_t &state : b) {
		bool hit = false;
		for (const size_t &other_state : a) {
			if (state == other_state) {
				hit = true;
				break;
			}
		}
		if (hit == false) { return false; }
	}

	// then they are equal
	return true;

}

void add_to_superposition(std::vector<size_t> &superposition, size_t new_state) noexcept {
	for (const size_t &state : superposition) {
		if (state == new_state) { return; }
	}

	superposition.push_back(new_state);
}

bool is_ghost_row(const nfa_table_t &nfa_table, size_t row) noexcept {
	return nfa_table.rows[row].ghost_row;
}

std::vector<size_t> follow_ghost_rows(const nfa_table_t &nfa_table, const std::vector<size_t> &state_superposition) noexcept {
	std::vector<size_t> result;

	for (size_t i = 0; i < state_superposition.size(); i++) {
		const size_t state = state_superposition[i];
		const nfa_row_t &nfa_row = nfa_table.rows[state];

		if (is_ghost_row(nfa_table, state)) {
			for (const auto &child : nfa_row.children) {
				result.push_back(child);
			}
			continue;
		}

		result.push_back(state);
	}

	return result;
}

size_t allocate_dfa_row(dfa_table_t &dfa_table) noexcept {
	dfa_table.rows.push_back({ });
	return dfa_table.rows.size() - 1;
}

void overwrite_dfa_row_next(dfa_table_t &dfa_table,
			    size_t row_index,
			    uint8_t character,
			    size_t target) noexcept {

	auto &row = dfa_table.rows[row_index];
	row.elements[character].next = target;

}

// NOTE: Assumes that follow_ghost_rows has already been called.
void shape_edges(dfa_table_t &dfa_table,
		     const nfa_table_t &nfa_table,
		     const std::vector<size_t> state_superposition,
		     size_t current_dfa_row) noexcept {

	// TODO: segmentation fault somewhere in here, find it and destroy it.

	std::vector<std::vector<size_t>> new_state_superpositions;
	std::vector<size_t> corresponding_nexts;

	for (uint16_t character = 0; character < 256; character++) {

	next_character:

		std::vector<size_t> new_state_superposition;

		for (const size_t &state : state_superposition) {
			const auto &element = nfa_table.rows[state].elements[character];
			if (element.next != 0) {
				add_to_superposition(new_state_superposition, element.next);
			}
		}

		if (new_state_superposition.empty()) {
			// if there is nowhere to go, mark a missing edge with 0
			overwrite_dfa_row_next(dfa_table, current_dfa_row, character, 0);
		}

		for (size_t i = 0; i < new_state_superpositions.size(); i++) {
			const auto &old_superposition = new_state_superpositions[i];

			if (superpositions_equal(old_superposition, new_state_superposition)) {
				overwrite_dfa_row_next(dfa_table, current_dfa_row, character,
						corresponding_nexts[i]);
				goto next_character;
			}
		}

		size_t new_dfa_row = allocate_dfa_row(dfa_table);

		new_state_superpositions.push_back(new_state_superposition);
		corresponding_nexts.push_back(new_dfa_row);

		overwrite_dfa_row_next(dfa_table, current_dfa_row, character, new_dfa_row);

		shape_edges(dfa_table, nfa_table, new_state_superposition, new_dfa_row);
	}
}

dfa_table_t gen_dfa(const nfa_table_t &nfa_table) noexcept {

	dfa_table_t result;

	std::vector<size_t> state_superposition;
	state_superposition.push_back(0);

	size_t origin_row = allocate_dfa_row(result);

	state_superposition = follow_ghost_rows(nfa_table, state_superposition);

	std::cout << "initial ghost rows followed:\n";
	for (const size_t &state : state_superposition) {
		std::cout << state;
	}
	std::cout << '\n';

	shape_edges(result, nfa_table, state_superposition, origin_row);

	return result;
}
