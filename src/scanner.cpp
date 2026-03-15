#include "scanner.h"
#include "error.h"
#include <unordered_map>

namespace fex {

namespace {

const std::unordered_map<std::string, TokenType> Keywords{
    {  "class",  CLASS },
    {   "else",   ELSE },
    {  "false",  FALSE },
    {    "for",    FOR },
    {     "if",     IF },
    {   "null",    NUL },
    {  "print",  PRINT },
    { "return", RETURN },
    {   "true",   TRUE },
    {    "var",    VAR },
    {  "while",  WHILE },
};

} // namespace

Scanner::Scanner(std::string_view source) : m_source(source), m_start(0), m_current(0), m_line(1) {}

const std::vector<Token>& Scanner::ScanTokens() {
    while (!IsAtEnd()) {
        m_start = m_current;
        ScanToken();
    }

    m_tokens.push_back({ END, "", {}, m_line });
    return m_tokens;
}

void Scanner::ScanToken() {
    char c = Advance();

    switch (c) {
        case '(': AddToken(LEFT_PAREN); break;
        case ')': AddToken(RIGHT_PAREN); break;
        case '{': AddToken(LEFT_BRACE); break;
        case '}': AddToken(RIGHT_BRACE); break;
        case ',': AddToken(COMMA); break;
        case '.': AddToken(DOT); break;
        case '-': AddToken(MINUS); break;
        case '+': AddToken(PLUS); break;
        case ';': AddToken(SEMICOLON); break;
        case '*': AddToken(STAR); break;
        case '/':
            if (Match('/')) {
                while (Peek() != '\n' && !IsAtEnd()) {
                    Advance();
                }
            } else {
                AddToken(SLASH);
            }
            break;
        case '%': AddToken(PERCENT); break;
        case '^': AddToken(CARET); break;
        case '~': AddToken(TILDE); break;
        case '!': AddToken(Match('=') ? EXCLAM_EQUAL : EXCLAM); break;
        case '=': AddToken(Match('=') ? EQUAL_EQUAL : EQUAL); break;
        case '<': AddToken(Match('=') ? LESS_EQUAL : LESS); break;
        case '>': AddToken(Match('=') ? GREATER_EQUAL : GREATER); break;
        case '&': AddToken(Match('&') ? AMP_AMP : AMP); break;
        case '|': AddToken(Match('|') ? PIPE_PIPE : PIPE); break;

        case '"': ScanString(); break;

        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace.
            break;

        case '\n': m_line++; break;
        default:
            if (IsDigit(c)) {
                ScanNumber();
            } else if (IsAlpha(c)) {
                ScanIdentifier();
            } else {
                FexError(m_line, "Unexpected character.");
            }
            break;
    }
}

void Scanner::ScanString() {
    while (Peek() != '"' && !IsAtEnd()) {
        if (Peek() == '\n') {
            m_line++;
        }
        Advance();
    }

    if (IsAtEnd()) {
        FexError(m_line, "Unterminated string.");
        return;
    }

    // The closing ".
    Advance();

    // Trim the surrounding quotes.
    std::string value(m_source.begin() + m_start + 1, m_source.begin() + m_current - 1);
    AddToken(STRING, value);
}

void Scanner::ScanNumber() {
    while (IsDigit(Peek())) {
        Advance();
    }

    // Look for a fractional part.
    if (Peek() == '.' && IsDigit(Peek(1))) {
        // Consume the "."
        Advance();

        while (IsDigit(Peek())) {
            Advance();
        }
    }

    std::string s(m_source.begin() + m_start, m_source.begin() + m_current);
    AddToken(NUMBER, std::stod(s));
}

void Scanner::ScanIdentifier() {
    while (IsAlphaNumeric(Peek())) {
        Advance();
    }

    std::string text(m_source.begin() + m_start, m_source.begin() + m_current);

    auto it = Keywords.find(text);
    if (it != Keywords.end()) {
        AddToken(it->second);
    } else {
        AddToken(IDENTIFIER);
    }
}

void Scanner::AddToken(TokenType type) {
    AddToken(type, {});
}

void Scanner::AddToken(TokenType type, Value literal) {
    std::string_view text(&m_source[m_start], m_current - m_start);
    m_tokens.emplace_back(type, text, literal, m_line);
}

bool Scanner::IsAtEnd(int offset) const {
    return m_current + offset >= m_source.length();
}
char Scanner::Peek(int offset) const {
    if (IsAtEnd(offset)) {
        return '\0';
    }
    return m_source[m_current + offset];
}

char Scanner::Advance() {
    return m_source[m_current++];
}

bool Scanner::Match(char expected) {
    if (IsAtEnd() || m_source[m_current] != expected) {
        return false;
    }

    m_current++;
    return true;
}

bool Scanner::IsDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Scanner::IsAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Scanner::IsAlphaNumeric(char c) {
    return IsAlpha(c) || IsDigit(c);
}

} // namespace fex
