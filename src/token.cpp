#include "pch.h"
#include "token.h"
#include "utils.h"

namespace fex {

namespace {

const std::unordered_map<TokenType, std::string> TypeToStr{
    // Single-character tokens
    {    LEFT_PAREN,    "left_paren" },
    {   RIGHT_PAREN,   "right_paren" },
    {    LEFT_BRACE,    "left_brace" },
    {   RIGHT_BRACE,   "right_brace" },
    {         COMMA,         "comma" },
    {           DOT,           "dot" },
    {         MINUS,         "minus" },
    {          PLUS,          "plus" },
    {     SEMICOLON,     "semicolon" },
    {         SLASH,         "slash" },
    {          STAR,          "star" },

    // One or two character tokens
    {        EXCLAM,        "exclam" },
    {  EXCLAM_EQUAL,  "exclam_equal" },
    {         EQUAL,         "equal" },
    {   EQUAL_EQUAL,   "equal equal" },
    {       GREATER,       "greater" },
    { GREATER_EQUAL, "greater_equal" },
    {          LESS,          "less" },
    {    LESS_EQUAL,    "less_equal" },
    {       AMP_AMP,   "logical_and" },
    {     PIPE_PIPE,    "logical_or" },

    // Literals
    {    IDENTIFIER,    "identifier" },
    {        STRING,        "string" },
    {        NUMBER,        "number" },

    // Keywords
    {         CLASS,       "keyword" },
    {          ELSE,       "keyword" },
    {         FALSE,       "keyword" },
    {           FOR,       "keyword" },
    {            IF,       "keyword" },
    {           NUL,       "keyword" },
    {         PRINT,       "keyword" },
    {        RETURN,       "keyword" },
    {          TRUE,       "keyword" },
    {           VAR,       "keyword" },
    {         WHILE,       "keyword" },

    {           END,           "end" }
};

} // namespace

std::string ValToStr(const Value& value) {
    return std::visit(
        [](const auto& val) -> std::string {
            using T = std::decay_t<decltype(val)>;
            if constexpr (std::is_same_v<T, std::string>) {
                return val;
            } else if constexpr (std::is_same_v<T, double>) {
                return FormatDouble(val);
            } else if constexpr (std::is_same_v<T, bool>) {
                return val ? "true" : "false";
            } else {
                return "";
            }
        },
        value);
}

Token::Token(TokenType type, std::string_view lexeme, const Value& literal, int line)
    : type(type), lexeme(lexeme), literal(literal), line(line) {}

std::string Token::ToStr() const {
    return std::string(TypeToStr.at(type)) + " " + lexeme + " " + ValToStr(literal);
}

} // namespace fex
