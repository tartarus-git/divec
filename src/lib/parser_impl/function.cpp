#include "parser.h"

#include <cstdlib>

#include "helpers.h"

// TODO: remove this later
#include <iostream>

// Taking these in as compile-time constants instead of as an array argument allows us to force the compiler into initializing
// the heap array efficiently. What was before a copy from memory to memory is now a series of stores from immediate values.
// The compiler might very well have optimized it to that anyway, even if we did do it with an array argument.
// But this is cooler anyway, and it doesn't rely on optimization. At least not in nearly the same way as the alternative would've.
template <dive_token_type_t... expected_token_types>
static divec_error_t push_unexpected_token_error_to_build_log(dive_build_log_t build_log,
							      const char *source_code,
							      const dive_token_t &token) noexcept
{

	dive_build_error_unexpected_token_t *entry = (dive_build_error_unexpected_token_t*)std::malloc(sizeof(dive_build_error_unexpected_token_t));
	if (entry == nullptr) { return divec_error_t::OUT_OF_MEMORY; }

	const auto [source_code_line, source_code_column] = helpers::get_line_column_from_text_position(source_code, token.begin);
	if (source_code_line == -1 || source_code_column == -1) { return divec_error_t::BUG; }

	dive_token_type_t *expected_token_types_storage = (dive_token_type_t*)std::malloc(sizeof...(expected_token_types) * sizeof(dive_token_type_t));
	{ size_t i = 0; [](...){}((expected_token_types_storage[i++] = expected_token_types)...); }	// this line is a work of art

	// start out with BUG in the error slot so that if the function forgets to set err, it'll count as a bug, because it is
	// I don't think I'll do this everywhere, but if I'm gonna initialize this to a value, it might as well be this one.
	divec_error_t err = divec_error_t::BUG;
	new (entry) dive_build_error_unexpected_token_t(source_code_line,
							source_code_column,
							source_code + token.begin,
							token.end - token.begin,
							expected_token_types_storage,
							sizeof...(expected_token_types),
							err);
	if (err != divec_error_t::SUCCESS) {
		std::free(entry);
		return err;
	}

	err = link_entry_to_build_log(build_log, entry);
	if (err != divec_error_t::SUCCESS) {
		std::free(entry);
		return err;
	}

	return divec_error_t::SUCCESS;

}

dive_ast_function_t* parser_t::parse_function(dive_build_log_t build_log, divec_error_t &err) noexcept {

	err = divec_error_t::SUCCESS;

	dive_ast_function_t *result = (dive_ast_function_t*)std::malloc(sizeof(dive_ast_function_t));
	if (result == nullptr) {
		err = divec_error_t::OUT_OF_MEMORY;
		return nullptr;
	}

	result->init();

	result->return_type = parse_type(build_log, err);
	if (err != divec_error_t::SUCCESS) {
		std::free(result);
		// TODO: consider calling free_children here.
		return nullptr;
	}

	std::cout << "finished parsing return type!\n";

	result->name = parse_identifier(build_log, err);
	if (err != divec_error_t::SUCCESS) {
		std::free(result);
		return nullptr;
	}

	std::cout << "finished parsing function name!\n";

	{
		const dive_token_t &token = get_next_token();
		if (token.type != dive_token_type_t::OPENING_PARENTHESIS) {

			err = divec_error_t::BUILD_ERROR;
			std::free(result);

			if (build_log != nullptr) {
				divec_error_t sub_err = push_unexpected_token_error_to_build_log<
												 dive_token_type_t::OPENING_PARENTHESIS
												>
												(
												 build_log,
												 program->source_code,
												 token
												);
				if (sub_err != divec_error_t::SUCCESS) { err = sub_err; return nullptr; }
			}

			return nullptr;
		}
	}

	std::cout << "finished parsing opening parenthesis!\n";

	if (peek_token(0).type != dive_token_type_t::CLOSING_PARENTHESIS) {

		while (true) {

			dive_ast_type_t argument_type = parse_type(build_log, err);
			if (err != divec_error_t::SUCCESS) {
				result->free_children();
				std::free(result);
				return nullptr;
			}

			std::cout << "parsed argument type!\n";

			dive_ast_identifier_t argument_name = parse_identifier(build_log, err);
			if (err != divec_error_t::SUCCESS) {
				result->free_children();
				std::free(result);
				return nullptr;
			}

			std::cout << "parsed argument identifier!\n";

			err = result->push_argument(argument_type, argument_name);
			if (err != divec_error_t::SUCCESS) {
				result->free_children();
				std::free(result);
				return nullptr;
			}

			std::cout << "pushed argument to function ast structure!\n";

			if (peek_token(0).type == dive_token_type_t::CLOSING_PARENTHESIS) { break; }

			{
				const dive_token_t &token = get_next_token();
				if (token.type != dive_token_type_t::COMMA) {

					err = divec_error_t::BUILD_ERROR;
					result->free_children();
					std::free(result);

					if (build_log != nullptr) {
						divec_error_t sub_err = push_unexpected_token_error_to_build_log<
														 dive_token_type_t::COMMA,
														 dive_token_type_t::CLOSING_PARENTHESIS
														>
														(
														 build_log,
														 program->source_code,
														 token
														);
						if (sub_err != divec_error_t::SUCCESS) { err = sub_err; return nullptr; }
					}

					return nullptr;
				}
			}

		}

	}

	std::cout << "finished parsing arguments!\n";

	// take parenthesis from queue
	get_next_token();

	if (peek_token(0).type != dive_token_type_t::OPENING_ANGEL_BRACKET) {
		err = divec_error_t::BUILD_ERROR;
		// TODO: append it to build log
		result->free_children();
		std::free(result);
		return nullptr;
	}

	std::cout << "got to just before statement parsing!\n";

	result->body = parse_statement(build_log, err);
	if (err != divec_error_t::SUCCESS) {
		result->free_children();
		std::free(result);
		return nullptr;
	}

	std::cout << "finished parsing statement!\n";

	return result;

}
