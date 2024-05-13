#include "dfa_gen.h"

#include "nfa_gen.h"

#include <vector>
#include <cstdint>
#include <iostream>

#include <cstdlib>

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

	// TODO: Is the following condition redundant given the first condition? Of course it is,
	// but I mean in this specific implementation. I don't know where we filter out duplicates here anymore.
	// This could've been done on purpose.

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

	bool hit = false;

	for (size_t i = 0; i < state_superposition.size(); i++) {
		const size_t state = state_superposition[i];
		const nfa_row_t &nfa_row = nfa_table.rows[state];

		if (is_ghost_row(nfa_table, state)) {

			if (nfa_row.children.empty()) {
				result.push_back(state);
				continue;
			}

			hit = true;

			for (const auto &child : nfa_row.children) {
				result.push_back(child);
			}
			continue;
		}

		result.push_back(state);
	}

	if (hit == true) {
		return follow_ghost_rows(nfa_table, result);
	}

	return result;
}

size_t allocate_dfa_row(dfa_table_t &dfa_table) noexcept {
	dfa_table.rows.push_back({ { }, (size_t)-1 });
	return dfa_table.rows.size() - 1;
}

void overwrite_dfa_row_next(dfa_table_t &dfa_table,
			    size_t row_index,
			    uint16_t character,
			    size_t target) noexcept {

	auto &row = dfa_table.rows[row_index];
	row.elements[character].next = target;

}

void overwrite_dfa_row_token_id(dfa_table_t &dfa_table,
				size_t row_index,
				size_t token_id) noexcept {

	auto &row = dfa_table.rows[row_index];
	row.token_id = token_id;

}

size_t shape_edges(dfa_table_t &dfa_table,
		     const nfa_table_t &nfa_table,
		     std::vector<size_t> state_superposition,
		     size_t current_dfa_row,
		     std::vector<std::vector<size_t>> &global_superposition_catalogue,
		     std::vector<size_t> &global_corresponding_nexts) noexcept {

	// TODO: This is SEMI-redundant because there exists another copy of it below somewhere.
	// Don't remove this without thinking about it though, because there are edge-cases.
	state_superposition = follow_ghost_rows(nfa_table, state_superposition);

	if (state_superposition.size() > 1) {
		std::cout << "greater than 1 found on:\n";
		for (const auto &state : state_superposition) {
			std::cout << state << ", ";
		}
		std::cout << '\n';
	}

	size_t terminator_id = -1;

	ssize_t highest_priority_thus_far = -1;

	// NOTE: ssize_t is needed because or else i-- can cause a bug to happen
	// where some things are ignored in the state_superposition vector.
	// I probably meant integer underflow when I wrote that note.
	for (ssize_t i = 0; i < state_superposition.size(); i++) {
		const size_t &state = state_superposition[i];
		if (is_ghost_row(nfa_table, state)) {
			if (nfa_table.rows[state].children.empty()) {

				if ((ssize_t)(nfa_table.rows[state].priority) > highest_priority_thus_far) {

					terminator_id = nfa_table.rows[state].token_id;
					highest_priority_thus_far = nfa_table.rows[state].priority;

				}
				state_superposition.erase(state_superposition.begin() + i);
				i--;
				continue;
			}
		}
	}

	overwrite_dfa_row_token_id(dfa_table, current_dfa_row, terminator_id);

	std::vector<std::vector<size_t>> new_state_superpositions;
	std::vector<size_t> corresponding_nexts;

	std::cout << '\n';

	for (uint16_t character = 0; character < 257; character++) {

		std::vector<size_t> new_state_superposition;

		for (const size_t &state : state_superposition) {
			const auto &element = nfa_table.rows[state].elements[character];
			if (element.next != 0) {
				add_to_superposition(new_state_superposition, element.next);
			}
		}

		std::cout << new_state_superposition.size();

		if (new_state_superposition.empty()) {
			// if there is nowhere to go, mark a missing edge with 0
			overwrite_dfa_row_next(dfa_table, current_dfa_row, character, 0);
			continue;
		}

		for (size_t i = 0; i < new_state_superpositions.size(); i++) {
			const auto &old_superposition = new_state_superpositions[i];

			if (superpositions_equal(old_superposition, new_state_superposition)) {
				overwrite_dfa_row_next(dfa_table, current_dfa_row, character,
						corresponding_nexts[i]);
				goto next_character;
			}
		}

		for (size_t i = 0; i < global_superposition_catalogue.size(); i++) {
			const auto &global_superposition = global_superposition_catalogue[i];

			if (superpositions_equal(new_state_superposition, global_superposition)) {
				overwrite_dfa_row_next(dfa_table, current_dfa_row, character, global_corresponding_nexts[i]);
				goto next_character;
			}
		}

		{

		new_state_superpositions.push_back(new_state_superposition);

			size_t new_dfa_row = -1;

			new_dfa_row = allocate_dfa_row(dfa_table);
			overwrite_dfa_row_next(dfa_table, current_dfa_row, character, new_dfa_row);
			corresponding_nexts.push_back(new_dfa_row);

			for (const auto &superposition : new_state_superpositions) {
				global_superposition_catalogue.push_back(superposition);
			}

			for (const size_t &next : corresponding_nexts) {
				global_corresponding_nexts.push_back(next);
			}

			shape_edges(dfa_table, nfa_table, new_state_superposition, new_dfa_row,
				global_superposition_catalogue, global_corresponding_nexts);

		}

	next_character:
	}

	std::cout << '\n';

	return terminator_id;
}

dfa_table_t gen_dfa(const nfa_table_t &nfa_table) noexcept {

	dfa_table_t result;

	std::vector<size_t> state_superposition;
	state_superposition.push_back(0);

	size_t origin_row = allocate_dfa_row(result);

	std::cout << "initial ghost rows followed (list of them):\n";
	for (const size_t &state : state_superposition) {
		std::cout << state;
	}
	std::cout << '\n';

	std::vector<std::vector<size_t>> global_cat;
	std::vector<size_t> global_nexts;

	size_t recursion_terminator_id = shape_edges(result, nfa_table, state_superposition, origin_row, global_cat, global_nexts);

	std::cout << recursion_terminator_id << '\n';

	if (recursion_terminator_id != -1) {
		std::cerr << "ONE OF THE REGEXES IS EMPTY (I THINK), NOT ALLOWED!\n";
		std::exit(EXIT_FAILURE);
	}

	return result;
}
