#pragma once

#include "expr.h"
#include <string_view>
#include <vector>

namespace fex {

class Arena;

class Parser {
  public:
    Parser(const std::vector<Token>& tokens, Arena& arena);
    const Expr* Parse();

  private:
    struct ParseError {};

    const Expr* Expression();
    const Expr* LogicalOr();
    const Expr* LogicalAnd();
    const Expr* InclusiveOr();
    const Expr* ExclusiveOr();
    const Expr* BitwiseAnd();
    const Expr* Equality();
    const Expr* Relational();
    const Expr* Term();
    const Expr* Factor();
    const Expr* Unary_();
    const Expr* Primary_();

    // helpers
    template <typename... Args>
    bool Match(Args... args);
    bool Check(TokenType type);
    Token Advance();
    bool IsAtEnd();
    Token Peek();
    Token Previous();

    Token Consume(TokenType type, std::string_view message);
    ParseError Error(Token token, std::string_view message);

    const std::vector<Token>& m_tokens;
    int m_current;
    Arena& m_arena;
};

} // namespace fex
