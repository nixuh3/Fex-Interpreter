#include "token.h"
#include <cmath>
#include <iomanip>
#include <sstream>
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
    {       PERCENT,           "percent" },

    // One or two character tokens
    {        EXCLAM,       "exclamation" },
    {  EXCLAM_EQUAL, "exclamation equal" },
    {         EQUAL,             "equal" },
    {   EQUAL_EQUAL,       "equal equal" },
    {       GREATER,           "greater" },
    { GREATER_EQUAL,     "greater equal" },
    {          LESS,              "less" },
    {    LESS_EQUAL,        "less equal" },
    {       AMP_AMP,       "logical and" },
    {     PIPE_PIPE,        "logical or" },

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

std::string FormatDouble(double x, int precision = 6) {
    if (std::isnan(x)) {
        return "nan";
    } else if (std::isinf(x)) {
        return x > 0 ? "inf" : "-inf";
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << x;

    std::string s = oss.str();

    auto pos = s.find_last_not_of('0');
    if (pos != std::string::npos) {
        s.erase(pos + 1);
    }

    if (!s.empty() && s.back() == '.') {
        s.pop_back();
    }

    return s;
}

} // namespace

std::string ValToStr(const Value& value) {
    return std::visit(
        [](auto&& val) -> std::string {
            using T = std::decay_t<decltype(val)>;
            if constexpr (std::is_same_v<T, std::string>) {
                return val;
            } else if constexpr (std::is_same_v<T, double>) {
                return FormatDouble(val);
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
