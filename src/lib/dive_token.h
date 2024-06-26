#pragma once

#include <cstddef>
#include <cstdint>

// Size is very conservative because I don't want to have to break API
// in the future if I add a bunch more tokens.
// This enum might not end up in a user-facing position,
// but the user will be able to receive tokens in some way,
// and those will come from this internal representation.
// So to make the "conversion" easier and to avoid possible bugs, this
// has a conservative width as well. If this ends
// up being user-facing, it'll have to have the conservative width anyway because
// API.
enum class dive_token_type_t : uint32_t {
	EOF_TOKEN,
	WHITESPACE,
	IF,
	WHILE,
	FOR,
	OPENING_PARENTHESIS,
	CLOSING_PARENTHESIS,
	OPENING_SQUARE_BRACKET,
	CLOSING_SQUARE_BRACKET,
	DOT,
	OPENING_ANGEL_BRACKET,
	CLOSING_ANGEL_BRACKET,
	FLOAT,
	DOUBLE,
	DECIMAL,
	INT32,
	INT64,
	UINT32_T,
	UINT64_T,
	INT16_T,
	UINT16_T,
	INT8_T,
	UINT8_T,
	NEWLINE,
	SEMICOLON,
	COMMA,
	IDENTIFIER,
	PLUS,
	MINUS,
	DIVIDE,
	ASTERISK,
	LESS_THAN,
	GREATER_THAN,
	LEFT_SHIFT,
	RIGHT_SHIFT,
	ASSIGNMENT_EQUAL,
	EQUALITY_EQUAL,
	ASSIGNMENT_PLUS,
	ASSIGNMENT_MINUS,
	ASSIGNMENT_DIVIDE,
	ASSIGNMENT_MULTIPLY,
	ASSIGNMENT_RIGHT_SHIFT,
	ASSIGNMENT_LEFT_SHIFT,
	BITWISE_AND,
	BITWISE_OR,
	BITWISE_XOR,
	BITWISE_NOT,
	LOGICAL_AND,
	LOGICAL_OR,
	LOGICAL_NOT,
	NOT_EQUAL,
	GREATER_THAN_OR_EQUAL,
	LESS_THAN_OR_EQUAL,
	BACKSLASH,
	TODO1,
	TODO2,
	VOID,
	INT32_LITERAL,
	INVALID_TOKEN
};

struct dive_token_t {
	dive_token_type_t type;
	size_t begin;
	size_t end;
};

const char* get_dive_token_type_string(dive_token_type_t token_type) noexcept;
