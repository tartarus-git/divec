#include "nfa_gen.h"

#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>

std::vector<regex_token_t> tokenize_regex(const std::string &regex) noexcept {
	std::vector<regex_token_t> result;
	for (size_t i = 0; i < regex.length(); i++) {

		char character = regex[i];

		switch (character) {
		case '(':
			result.push_back({ regex_token_type_t::SUBEXPRESSION_BEGIN });
			break;
		case ')':
			result.push_back({ regex_token_type_t::SUBEXPRESSION_END });
			break;
		case '|':
			result.push_back({ regex_token_type_t::ALTERNATION });
			break;
		case '*':
			result.push_back({ regex_token_type_t::KLEENE_STAR });
			break;
		// TODO: implement tokenization of [] expressions as filters.

		case '\\':
			i++;

			character = regex[i];
			switch (character) {

			case 't':
				character = '\t';
				break;
			case '0':
				character = '\0';
				break;
			case 'n':
				character = '\n';
				break;

			}

			// FALLTHROUGH

		default:
			regex_token_t new_token = { regex_token_type_t::FILTER };
			new_token.row_filter[(uint8_t)character] = true;
			result.push_back(new_token);
			break;
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
	std::vector<size_t> ghost_row_source_stack;
	ghost_row_source_stack.push_back(0);

	std::vector<size_t> ghost_row_destination_stack;
	size_t ending_ghost_row = allocate_ghost_row(nfa_table);
	nfa_table.rows[ending_ghost_row].token_id = token_id;
	ghost_row_destination_stack.push_back(ending_ghost_row);

	size_t last_row = allocate_nfa_row(nfa_table, { false, { } });
	patch_from_ghost_row(nfa_table, 0, last_row);

	size_t just_popped_source = -1;
	size_t just_popped_destination = -1;

	std::cout << last_row << '\n';

	std::vector<regex_token_t> regex_tokens = tokenize_regex(regex);

	for (const regex_token_t &regex_token : regex_tokens) {

		switch (regex_token.type) {

			case regex_token_type_t::FILTER:
				std::cout << "FILTER\n";
				{
				size_t new_last_row = allocate_nfa_row(nfa_table, { false, { } });
				shape_row(nfa_table, last_row, regex_token.row_filter, new_last_row);
				last_row = new_last_row;
				break;
				}

			case regex_token_type_t::ALTERNATION:
				std::cout << "ALTERNATION\n";
				convert_to_ghost_row(nfa_table, last_row);
				patch_from_ghost_row(nfa_table, last_row, *(ghost_row_destination_stack.end() - 1));
				last_row = allocate_nfa_row(nfa_table, { false, { } });
				std::cout << last_row << '\n';
				patch_from_ghost_row(nfa_table, *(ghost_row_source_stack.end() - 1), last_row);
				break;

			case regex_token_type_t::SUBEXPRESSION_BEGIN:
				std::cout << "SUBEXPRESSION_BEGIN\n";
				convert_to_ghost_row(nfa_table, last_row);
				ghost_row_source_stack.push_back(last_row);
				ghost_row_destination_stack.push_back(allocate_ghost_row(nfa_table));
				last_row = allocate_nfa_row(nfa_table, { false, { } });
				patch_from_ghost_row(nfa_table, ghost_row_source_stack.back(), last_row);
				break;

			case regex_token_type_t::SUBEXPRESSION_END:
				std::cout << "SUBEXPRESSION_END\n";
				{
				just_popped_source = ghost_row_source_stack.back();
				ghost_row_source_stack.pop_back();
				size_t temp_ghost_row = ghost_row_destination_stack.back();
				just_popped_destination = temp_ghost_row;
				ghost_row_destination_stack.pop_back();

				convert_to_ghost_row(nfa_table, last_row);
				patch_from_ghost_row(nfa_table, last_row, temp_ghost_row);

				last_row = allocate_nfa_row(nfa_table, { false, { } });
				patch_from_ghost_row(nfa_table, temp_ghost_row, last_row);
				break;
				}

				// TODO: Kleene star only works directly after parenthesis
				// right now. Fix that in future version of project.
			case regex_token_type_t::KLEENE_STAR:
				std::cout << "KLEENE_STAR\n";
				{
				size_t newest_end_ghost_row = just_popped_destination;
				size_t newest_start_ghost_row = just_popped_source;
				patch_from_ghost_row(nfa_table, newest_end_ghost_row, newest_start_ghost_row);
				patch_from_ghost_row(nfa_table, newest_start_ghost_row, last_row);
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
		if (line.empty()) { continue; }

		std::vector<std::string> parts = split(line, "->");

		size_t token_id = std::atoi(parts[0].c_str());
		const std::string &regex = parts[1];
		std::cout << token_id << ", " << regex << '\n';

		add_nfa_rows_for_regex(result, token_id, regex);
	}

	return result;
}
