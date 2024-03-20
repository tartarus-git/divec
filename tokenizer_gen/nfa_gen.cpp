#include "nfa_gen.h"

#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

std::vector<regex_token_t> tokenize_regex(const std::string &regex) noexcept {
	std::vector<regex_token_t> result;
	for (const char &character : regex) {
		switch (character) {
		case '(':
			result.push_back({ regex_token_type_t::SUBEXPRESSION_BEGIN });
		case ')':
			result.push_back({ regex_token_type_t::SUBEXPRESSION_END });
		case '|':
			result.push_back({ regex_token_type_t::ALTERNATION });
		case '*':
			result.push_back({ regex_token_type_t::KLEENE_STAR });
		// TODO: implement tokenization of [] expressions as filters.
		default:
			result.push_back({ regex_token_type_t::FILTER });
		}
	}
	return result;
}

std::vector<std::string> split(std::string input, std::string delimiter) noexcept {
	std::vector<std::string> result;

	size_t last_delimiter = -delimiter.length();
	while (true) {
		size_t new_delimiter = input.find(delimiter, last_delimiter + delimiter.length());

		if (new_delimiter == std::string::npos) {
			result.push_back(input.substr(last_delimiter + delimiter.length()));
			break;
		}

		std::string next_section = input.substr(last_delimiter + delimiter.length(), new_delimiter - (last_delimiter + delimiter.length()));
		last_delimiter = new_delimiter;
		result.push_back(next_section);
	}

	return result;
}

size_t allocate_nfa_row(nfa_table_t &nfa_table, const nfa_row_t &row) noexcept {
	nfa_table.rows.push_back(row);
	return nfa_table.rows.size() - 1;
}

size_t allocate_ghost_row(nfa_table_t &nfa_table) noexcept {
	return allocate_nfa_row(nfa_table, { true, { } });
}

void convert_to_ghost_row(nfa_table_t &nfa_table, size_t ghost_row) noexcept {
	nfa_row_t &row = nfa_table.rows[ghost_row];
	row.ghost_row = true;
	// NOTE: Assumes children vector is empty already.
}

void patch_from_ghost_row(nfa_table_t &nfa_table, size_t ghost_row, size_t target) noexcept {
	nfa_table.rows[ghost_row].children.push_back(target);
}

void shape_row(nfa_table_t &nfa_table, size_t row, const bool (&row_filter)[256], size_t target) noexcept {
	for (uint16_t i = 0; i < 256; i++) {
		if (row_filter[i] == true) {
			nfa_table.rows[row].elements[i].next = target;
		}
	}
}

void add_nfa_rows_for_regex(nfa_table_t &nfa_table, size_t token_id, const std::string &regex) noexcept {
	size_t last_row = allocate_nfa_row(nfa_table, { false, { } });
	patch_from_ghost_row(nfa_table, 0, last_row);

	std::vector<size_t> ghost_row_source_stack;
	ghost_row_source_stack.push_back(0);

	std::vector<size_t> ghost_row_destination_stack;
	size_t ending_ghost_row = allocate_ghost_row(nfa_table);
	nfa_table.rows[ending_ghost_row].token_id = token_id;
	ghost_row_destination_stack.push_back(ending_ghost_row);

	std::vector<regex_token_t> regex_tokens = tokenize_regex(regex);

	for (const regex_token_t &regex_token : regex_tokens) {

		switch (regex_token.type) {

			case regex_token_type_t::FILTER:
				{
				size_t new_last_row = allocate_nfa_row(nfa_table, { false, { } });
				shape_row(nfa_table, last_row, regex_token.row_filter, new_last_row);
				last_row = new_last_row;
				break;
				}

			case regex_token_type_t::ALTERNATION:
				convert_to_ghost_row(nfa_table, last_row);
				patch_from_ghost_row(nfa_table, last_row, *(ghost_row_destination_stack.end() - 1));
				last_row = allocate_nfa_row(nfa_table, { false, { } });
				patch_from_ghost_row(nfa_table, *(ghost_row_source_stack.end() - 1), last_row);
				break;

			case regex_token_type_t::SUBEXPRESSION_BEGIN:
				convert_to_ghost_row(nfa_table, last_row);
				ghost_row_source_stack.push_back(last_row);
				ghost_row_destination_stack.push_back(allocate_ghost_row(nfa_table));
				last_row = allocate_nfa_row(nfa_table, { false, { } });

			case regex_token_type_t::SUBEXPRESSION_END:
				{
				ghost_row_source_stack.pop_back();
				size_t temp_ghost_row = ghost_row_destination_stack.back();
				ghost_row_destination_stack.pop_back();
				last_row = allocate_nfa_row(nfa_table, { false, { } });
				patch_from_ghost_row(nfa_table, temp_ghost_row, last_row);
				break;
				}

			case regex_token_type_t::KLEENE_STAR:
				{
				size_t newest_end_ghost_row = *(ghost_row_destination_stack.end() - 1);
				size_t newest_start_ghost_row = *(ghost_row_source_stack.end() - 1);
				patch_from_ghost_row(nfa_table, newest_end_ghost_row, newest_start_ghost_row);
				break;
				}

		}

	}

	convert_to_ghost_row(nfa_table, last_row);
	patch_from_ghost_row(nfa_table, last_row, *(ghost_row_destination_stack.end() - 1));
}

nfa_table_t gen_nfa(const std::string &specification) noexcept {
	std::vector<std::string> lines = split(specification, "\n");

	nfa_table_t result;

	allocate_nfa_row(result, { true, { } });

	for (const std::string &line : lines) {
		std::vector<std::string> parts = split(line, "->");

		size_t token_id = std::atoi(parts[0].c_str());
		const std::string &regex = parts[1];

		add_nfa_rows_for_regex(result, token_id, regex);
	}

	return result;
}
