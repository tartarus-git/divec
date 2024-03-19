#include <fstream>
#include <sstream>
#include <string>

#include "nfa_gen.h"

#define SPECIFICATION_PATH "tokenizer.tokenizer_spec"

std::string get_specification() noexcept {
	std::ifstream spec_file(SPECIFICATION_PATH);
	std::stringstream buffer;
	buffer << spec_file.rdbuf();
	return buffer.str();
}

int main() {
	std::string specification = get_specification();

	nfa_table_t nfa = gen_nfa(specification);
}
