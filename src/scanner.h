#pragma once

#include "token.h"
#include <string_view>
#include <vector>

namespace fex {

class Scanner {
  public:
    Scanner(std::string_view source);

    const std::vector<Token>& ScanTokens();

  private:
    void ScanToken();
    void ScanString();
    void ScanNumber();
    void ScanIdentifier();

    void AddToken(TokenType type);
    void AddToken(TokenType type, Value literal);

    bool IsAtEnd(int offset = 0) const;
    char Peek(int offset = 0) const;
    char Advance();
    bool Match(char expected);

    static bool IsDigit(char c);
    static bool IsAlpha(char c);
    static bool IsAlphaNumeric(char c);

    std::string_view m_source;
    std::vector<Token> m_tokens;

    int m_start;
    int m_current;
    int m_line;
};

} // namespace fex
