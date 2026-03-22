#pragma once

#include <string>
#include <variant>

namespace fex {

enum TokenType {
    // Single-character tokens
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,
    PERCENT,

    // One or two character tokens
    EXCLAM,
    EXCLAM_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,
    AMP_AMP,
    PIPE_PIPE,

    // Literals
    IDENTIFIER,
    STRING,
    NUMBER,

    // Keywords
    CLASS,
    ELSE,
    FALSE,
    FOR,
    IF,
    NUL,
    PRINT,
    RETURN,
    TRUE,
    VAR,
    WHILE,

    END
};

using Value = std::variant<std::monostate, double, std::string>;

std::string ValToStr(const Value& value);

struct Token {
    Token(TokenType type, std::string_view lexeme, const Value& literal, int line);

    std::string ToStr() const;

    const TokenType type;
    const std::string lexeme;
    const Value literal;
    const int line;
};

} // namespace fex
