#include "token.h"
#include <unordered_map>

namespace fex {

namespace {

const std::unordered_map<TokenType, std::string> TypeToStr{
    // Single-character tokens
    {    LEFT_PAREN,  "left parenthesis" },
    {   RIGHT_PAREN, "right parenthesis" },
    {    LEFT_BRACE,        "left brace" },
    {   RIGHT_BRACE,       "right brace" },
    {         COMMA,             "comma" },
    {           DOT,               "dot" },
    {         MINUS,             "minus" },
    {          PLUS,              "plus" },
    {     SEMICOLON,         "semicolon" },
    {         SLASH,             "slash" },
    {          STAR,              "star" },

    // One or two character tokens
    {        EXCLAM,       "exclamation" },
    {  EXCLAM_EQUAL, "exclamation equal" },
    {         EQUAL,             "equal" },
    {   EQUAL_EQUAL,       "equal equal" },
    {       GREATER,           "greater" },
    { GREATER_EQUAL,     "greater equal" },
    {          LESS,              "less" },
    {    LESS_EQUAL,        "less equal" },
    {           AMP,       "bitwise and" },
    {       AMP_AMP,       "logical and" },
    {          PIPE,        "bitwise or" },
    {     PIPE_PIPE,        "logical or" },
    {         CARET,               "xor" },
    {         TILDE,               "not" },

    // Literals
    {    IDENTIFIER,        "identifier" },
    {        STRING,            "string" },
    {        NUMBER,            "number" },

    // Keywords
    {         CLASS,             "class" },
    {          ELSE,              "else" },
    {         FALSE,             "false" },
    {           FOR,               "for" },
    {            IF,                "if" },
    {           NUL,              "null" },
    {         PRINT,             "print" },
    {        RETURN,            "return" },
    {          TRUE,              "true" },
    {           VAR,               "var" },
    {         WHILE,             "while" },

    {           END,               "EOF" }
};

} // namespace

Token::Token(TokenType type, std::string_view lexeme, const Literal& literal, int line)
    : m_type(type), m_lexeme(lexeme), m_literal(literal), m_line(line) {}

std::string Token::ToString() const {
    return std::string(TypeToStr.at(m_type)) + " " + m_lexeme + " " +
        std::visit(
               [](auto&& val) -> std::string {
                   using T = std::decay_t<decltype(val)>;
                   if constexpr (std::is_same_v<T, std::string>) {
                       return val;
                   } else if constexpr (std::is_same_v<T, double>) {
                       return std::to_string(val);
                   } else {
                       return "";
                   }
               },
               m_literal);
}

} // namespace fex
