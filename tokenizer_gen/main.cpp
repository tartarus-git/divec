#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include "nfa_gen.h"
#include "dfa_gen.h"

#define SPECIFICATION_PATH "tokenizer.tokenizer_spec"

std::string get_specification() noexcept {
	std::ifstream spec_file(SPECIFICATION_PATH);
	std::stringstream buffer;
	buffer << spec_file.rdbuf();
	return buffer.str();
}

void output_table_to_file(const dfa_table_t &table, const char *path) noexcept {
	std::ofstream table_file;
	table_file.open(path, std::ios::out | std::ios::binary);

	table_file.write((const char*)(table.rows.data()), table.rows.size() * sizeof(dfa_row_t));

	table_file.close();
}

int main() {
	std::string specification = get_specification();
	std::cout << specification << '\n';

	nfa_table_t nfa = gen_nfa(specification);

	for (const auto &row : nfa.rows) {
		for (const auto &element : row.elements) {
			std::cout << element.next;
		}
		std::cout << " ; " << row.token_id;
		std::cout << " ; ";
		for (const auto &child : row.children) {
			std::cout << child << ", ";
		}
		std::cout << " ; " << row.ghost_row;
		std::cout << '\n';
	}

	std::cout << "\nNEXT IS DFA OUTPUT:\n\n";

	dfa_table_t dfa = gen_dfa(nfa);

	std::cout << "\n\n\n";

	size_t i = 0;
	for (const auto &row : dfa.rows) {

		std::cout << i << "  ";

		for (const auto &element : row.elements) {

			std::cout << element.next << ':';
		}

		std::cout << " ; " << row.token_id;

		std::cout << '\n';
		i++;
	}

	output_table_to_file(dfa, ".temp_tokenizer_table");
}
