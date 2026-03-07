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
    CARET,
    TILDE,

    // One or two character tokens
    EXCLAM,
    EXCLAM_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,
    AMP,
    AMP_AMP,
    PIPE,
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

using Literal = std::variant<std::monostate, double, std::string>;

class Token {
  public:
    Token(TokenType type, std::string_view lexeme, const Literal& literal, int line);
    std::string ToString() const;

    TokenType GetType() const { return m_type; }
    int GetLine() const { return m_line; }
    const std::string& GetLexeme() const { return m_lexeme; }

  private:
    const TokenType m_type;
    const std::string m_lexeme;
    const Literal m_literal;
    const int m_line;
};

} // namespace fex
